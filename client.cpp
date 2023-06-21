#include "client.h"
#include "ChatItem.h"

Client::Client(QObject* parent)
    : QObject(parent)
    , logged_in(false)
{
    initSocket();
}

Client::~Client() {}

Client* Client::instance(QObject* parent)
{
    static Client inst(parent);
    return &inst;
}

const QTcpSocket* Client::socketInfo()
{
    return client_socket;
}

void Client::initSocket()
{
    client_socket = new QTcpSocket(this);
    // Forward the connected and disconnected signals
    connect(client_socket, &QTcpSocket::connected, this, &Client::connected);
    connect(client_socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(client_socket, &QTcpSocket::disconnected, client_socket, &QTcpSocket::deleteLater);

    // connect readyRead() to the slot that will take care of reading the data in
    connect(client_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);

    // Forward the error signal, QOverload is necessary as error() is overloaded, see the Qt docs 
    connect(client_socket, &QAbstractSocket::errorOccurred, this, &Client::errorSignal);

    // Reset the m_loggedIn variable when we disconnect. Since the operation is trivial we use a lambda instead of creating another slot
    connect(client_socket, &QTcpSocket::disconnected, this, [this]()->void {logged_in = false; });
}

void Client::connectToServer(const QHostAddress& address, quint16 port)
{
    client_socket->connectToHost(address, port);
    emit connected();
}
//login

void Client::login(const QString& userNickname_, const QString& userPassword_)
{
    // Create the JSON we want to send
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("login");
    message[QStringLiteral("username")] = userNickname_;
    message[QStringLiteral("password")] = userPassword_;
    // send the JSON
    sendJson(message);
}

//profile
void Client::createUser(QSharedPointer<DTOUser> shp_dto_user_)
{

    if (shp_dto_user_->getNickname().isEmpty())
        return; 

    QJsonObject user;
    user[QStringLiteral("type")] = QStringLiteral("signin");
    user[QStringLiteral("username")] = shp_dto_user_->getNickname();
    user[QStringLiteral("password")] = shp_dto_user_->getPassword();
    user[QStringLiteral("userpic")] = shp_dto_user_->getUserpic();

    sendJson(user);
}

void Client::updateUserPic(QSharedPointer<DTOUser> shp_dto_user_)
{
    if (shp_dto_user_->getUserpic().isEmpty())
        return;

    QJsonObject user;
    user[QStringLiteral("type")] = QStringLiteral("changeUserPic");
    user[QStringLiteral("nickname")] = shp_dto_user_->getNickname();
    user[QStringLiteral("userpic")] = shp_dto_user_->getUserpic();

    sendJson(user);
}

void Client::updateUserPassword(QSharedPointer<DTOUser> shp_dto_user_)
{
    if (shp_dto_user_->getPassword().isEmpty())
        return;

    QJsonObject user;
    user[QStringLiteral("type")] = QStringLiteral("changePassword");
    user[QStringLiteral("username")] = shp_dto_user_->getNickname();
    user[QStringLiteral("password")] = shp_dto_user_->getPassword();

    sendJson(user);
}

void Client::roomListRequest()
{
    // Create the JSON we want to send
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("roomListRequest");
    // send the JSON
    sendJson(message);
}

//chatList
void Client::askUserInfo(const QString& nickname_)
{
    QJsonObject user;
    user[QStringLiteral("type")] = QStringLiteral("askCurrentUser");
    user[QStringLiteral("nickname")] = nickname_;

    sendJson(user);
}

void Client::enterRoom(quint16 room_number_)
{
    if (room_number_ == 0)
        return;

    QJsonObject room;
    room[QStringLiteral("type")] = QStringLiteral("roomEntry");
    room[QStringLiteral("room")] = room_number_;

    sendJson(room);
}


//addChat
void Client::createChat(const chatItemPtr chat_)
{
    if (chat_->getChatRoomName().isEmpty() )
        return;

    QJsonObject chat_body;
    chat_body[QStringLiteral("name")] = chat_->getChatRoomName();
    chat_body[QStringLiteral("description")] = chat_->getChatRoomDescription();
    chat_body[QStringLiteral("topic")] = chat_->getChatRoomTopicName();
    chat_body[QStringLiteral("is_private")] = chat_->getChatRoomIsPrivate();
    chat_body[QStringLiteral("password")] = chat_->getChatRoomPassword();

    QJsonObject user_info;
    user_info[QStringLiteral("type")] = QStringLiteral("createRoom");
    user_info[QStringLiteral("chatbody")] = chat_body;

    sendJson(user_info);
}

//messageRoom
void Client::sendMessage(QSharedPointer<DTOMessage> shp_dto_message_)
{
    if (shp_dto_message_->getMessageText().isEmpty())
        return; // We don't send empty messages
    // Create the JSON we want to send

    QJsonObject messagebody;
    messagebody[QStringLiteral("id")] = shp_dto_message_->getMessageId();
    messagebody[QStringLiteral("parentid")] = "";
    messagebody[QStringLiteral("datetime")] = QDateTime::currentDateTime().toString();
    messagebody[QStringLiteral("nickname")] = shp_dto_message_->getMessageNickname();
    messagebody[QStringLiteral("text")] = shp_dto_message_->getMessageText();
    messagebody[QStringLiteral("mediaid")] = "";
    messagebody[QStringLiteral("rtl")] = shp_dto_message_->getRTL();
    messagebody[QStringLiteral("likes")] = QJsonObject();

    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("message");
    message[QStringLiteral("messagebody")] = messagebody;
    sendJson(message);
}

void Client::exitRoom()
{
    QJsonObject room;
    room[QStringLiteral("type")] = QStringLiteral("roomOut");

    sendJson(room);
}






void Client::sendJson(const QJsonObject& doc, const QString&  type, const QByteArray& data_)
{
    if (client_socket->state() == QAbstractSocket::ConnectedState) { // if the client is connected
        QByteArray buffer;
        buffer.clear();
        // create a QDataStream for buffer operating 
        QDataStream clientStream(&buffer, QIODevice::WriteOnly);
        // set the version so that programs compiled with different versions of Qt can agree on how to serialise
        clientStream.setVersion(QDataStream::Qt_6_5);
        
        // reserv size part in stream and send the JSON using QDataStream
        const QByteArray jsonData = QJsonDocument(doc).toJson();
        clientStream << quint16(0) << type<< jsonData <<data_;
        clientStream.device()->seek(0); //go to beginning data storage
        clientStream << quint16(buffer.size() - sizeof(quint16));
        client_socket->write(buffer);
    }
}

void Client::disconnectFromHost()
{
    client_socket->disconnectFromHost();
}

void Client::jsonReceived(const QJsonObject& docObj)
{
    // actions depend on the type of message
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return; // a message with no type was received so we just ignore it

    if (typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) == 0) { //It's a login message
        if (logged_in)
            return; // if we are already logged in we ignore
        // the success field will contain the result of our attempt to login
        const QJsonValue resultVal = docObj.value(QLatin1String("success"));
        if (resultVal.isNull() || !resultVal.isBool())
            return; // the message had no success field so we ignore
        const bool loginSuccess = resultVal.toBool();
        if (loginSuccess) {
            // we logged in succesfully and we notify it via the loggedIn signal
            {
                //TODO here should recive a full data of the user. That client will use for showing
                const QJsonObject user_info_json = docObj.value(QLatin1String("userinfo")).toObject();
                if (user_info_json.isEmpty())
                    return; // the userinfo has to not empty so we ignore
                const QJsonValue usernameVal = user_info_json.value(QLatin1String("username"));
                if (usernameVal.isNull() || !usernameVal.isString())
                    return; // the username was invalid so we ignore
                const QJsonValue userPic = user_info_json.value(QLatin1String("userpic"));
                const QJsonValue userRating = user_info_json.value(QLatin1String("rating"));

                user_nickname = usernameVal.toString();
                
                user_pic = userPic.toString();
                user_rating = userRating.toInt();
                //emit loggedIn({ usernameVal.toString(), userRating.toInt(), userPic.toString().toUtf8()}); //dtoUser (base64)
                emit loggedIn(DTOUser::DTOUser(usernameVal.toString(), userRating.toInt(), userPic.toString()));
            }
            return;
        }
        // the login attempt failed, we extract the reason of the failure from the JSON
        // and notify it via the loginError signal
        const QJsonValue reasonVal = docObj.value(QLatin1String("reason"));
        emit loginError(reasonVal.toString());
    }

    if (typeVal.toString().compare(QLatin1String("roomList"), Qt::CaseInsensitive) == 0)
    {
        const QJsonArray roomsVal = docObj.value(QLatin1String("rooms")).toArray();
        if (roomsVal.isEmpty())
            return; // rooms empty so we ignored
        chatList roomItems;
        for(QJsonValue room: roomsVal)
        {
            const QJsonObject roomObj = room.toObject();
            qint32 id = roomObj.value(QLatin1String("id")).toInt();
            QString name = roomObj.value(QLatin1String("name")).toString();
            QString description = roomObj.value(QLatin1String("description")).toString();
            QString topic = roomObj.value(QLatin1String("topic")).toString();
            bool is_private = roomObj.value(QLatin1String("is_private")).toBool();
            roomItems.append(chatItemPtr( new ChatItem( id, name, description, topic, is_private)));
        }
        emit chatListRecived(roomItems);

    }

    if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) == 0)
    {
        const QJsonObject messagebody_val = docObj.value(QLatin1String("messagebody")).toObject();
        if (messagebody_val.isEmpty())
            return;

        DTOMessage msg_;
        if (!DTOMessage::toDTOMessageFromJson(msg_, messagebody_val))
            return;
        emit messageReceived(msg_);
        return;
    }


    else if (typeVal.toString().compare(QLatin1String("newuser"), Qt::CaseInsensitive) == 0) { // A user joined the chat
        // we extract the username of the new user
        const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
        if (usernameVal.isNull() || !usernameVal.isString())
            return; // the username was invalid so we ignore
        // we notify of the new user via the userJoined signal
        emit userJoined(usernameVal.toString());
    }

    else if (typeVal.toString().compare(QLatin1String("userdisconnected"), Qt::CaseInsensitive) == 0) { // A user left the chat
        // we extract the username of the new user
        const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
        if (usernameVal.isNull() || !usernameVal.isString())
            return; // the username was invalid so we ignore
        // we notify of the user disconnection the userLeft signal
        emit userLeft(usernameVal.toString());
    }

    else if (typeVal.toString().compare(QLatin1String("myUserData"), Qt::CaseInsensitive) == 0) { // A user joined the chat
        const QJsonObject user_info_json = docObj.value(QLatin1String("userinfo")).toObject();
        if (user_info_json.isEmpty())
            return; // the userinfo has to not empty so we ignore
        const QJsonValue usernameVal = user_info_json.value(QLatin1String("username"));
        if (usernameVal.isNull() || !usernameVal.isString())
            return; // the username was invalid so we ignore
        const QJsonValue userPic = user_info_json.value(QLatin1String("userpic"));
        const QJsonValue userRating = user_info_json.value(QLatin1String("rating"));

        user_nickname = usernameVal.toString();
        user_pic = userPic.toString();
        user_rating = userRating.toInt();
        //emit loggedIn({ usernameVal.toString(), userRating.toInt(), userPic.toString().toUtf8()}); //dtoUser (base64)
        emit userInfoComed(DTOUser::DTOUser(usernameVal.toString(), userRating.toInt(), userPic.toString()));
    }

    else if (typeVal.toString().compare(QLatin1String("userpicChanged"), Qt::CaseInsensitive) == 0) { // A user joined the chat
        const QJsonObject user_info_json = docObj.value(QLatin1String("userinfo")).toObject();
        if (user_info_json.isEmpty())
            return; // the userinfo has to not empty so we ignore
        const QJsonValue usernameVal = user_info_json.value(QLatin1String("username"));
        if (usernameVal.isNull() || !usernameVal.isString())
            return; // the username was invalid so we ignore
        const QJsonValue userPic = user_info_json.value(QLatin1String("userpic"));
        const QJsonValue userRating = user_info_json.value(QLatin1String("rating"));

        user_nickname = usernameVal.toString();
        user_pic = userPic.toString();
        user_rating = userRating.toInt();
        //emit loggedIn({ usernameVal.toString(), userRating.toInt(), userPic.toString().toUtf8()}); //dtoUser (base64)
        emit userInfoComed(DTOUser::DTOUser(usernameVal.toString(), userRating.toInt(), userPic.toString()));
    }

    else if (typeVal.toString().compare(QLatin1String("passwordChanged"), Qt::CaseInsensitive) == 0) { // A user joined the chat
        const QJsonObject user_info_json = docObj.value(QLatin1String("userinfo")).toObject();
        if (user_info_json.isEmpty())
            return; // the userinfo has to not empty so we ignore
        const QJsonValue usernameVal = user_info_json.value(QLatin1String("username"));
        if (usernameVal.isNull() || !usernameVal.isString())
            return; // the username was invalid so we ignore
        const QJsonValue userPic = user_info_json.value(QLatin1String("userpic"));
        const QJsonValue userRating = user_info_json.value(QLatin1String("rating"));

        user_nickname = usernameVal.toString();
        user_pic = userPic.toString();
        user_rating = userRating.toInt();
        //emit loggedIn({ usernameVal.toString(), userRating.toInt(), userPic.toString().toUtf8()}); //dtoUser (base64)
        emit userInfoComed(DTOUser::DTOUser(usernameVal.toString(), userRating.toInt(), userPic.toString()));
    }

    else if (typeVal.toString().compare(QLatin1String("exitRoom"), Qt::CaseInsensitive) == 0) { // A user left the chat
        // we extract the username of the new user
        const QJsonValue result = docObj.value(QLatin1String("success"));
        emit roomWasExit(result.toString());
    };

}


void Client::onReadyRead()
{
    // prepare a container to hold the UTF-8 encoded JSON we receive from the socket
    QString type;
    QByteArray jsonData;
    // create a QDataStream operating on the socket
    QDataStream socketStream(client_socket);
    // set the version so that programs compiled with different versions of Qt can agree on how to serialise
    socketStream.setVersion(QDataStream::Qt_6_5);
    // start an infinite loop
    for (;;) {
        // get size of data
        if (nextBlockSize == 0)
        {
            if (client_socket->bytesAvailable() < 2)
            {
                break;
            }
            socketStream >> nextBlockSize;
        }
        // if avalable bytes less than size of data we exit the loop and wait for more data to become available
        if (client_socket->bytesAvailable() < nextBlockSize)
        {
            break;
        }
        // we start a transaction so we can revert to the previous state in case we try to read more data than is available on the socket
        socketStream.startTransaction();
        // we try to read the JSON data
        socketStream >> jsonData;
        if (socketStream.commitTransaction()) {
            // we successfully read some data
            // we now need to make sure it's in fact a valid JSON
            QJsonParseError parseError;
            // we try to create a json document with the data we received
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if (parseError.error == QJsonParseError::NoError) {
                // if the data was indeed valid JSON
                if (jsonDoc.isObject()) // and is a JSON object
                    jsonReceived(jsonDoc.object()); // parse the JSON
            }
            // loop and try to read more JSONs if they are available
        }
        else {
            // the read failed, the socket goes automatically back to the state it was in before the transaction started
            // we just exit the loop and wait for more data to become available
            break;
        }
        nextBlockSize = 0;
        break;
    }
}



//------------getters-----------
const QString& Client::getUserNickname() { return user_nickname; }

const QString& Client::getUserPassword() { return user_password; }

quint16 Client::getRoomNum() { return user_cur_room_number; }

const QString& Client::getUserAvatarPath() { return user_avatar_path; }

int Client::getUserRating() { return user_rating; }


//------------setters-----------
void Client::setUserName(QString user_name_)
{
    this->user_nickname = user_name_;
}

void Client::setUserPassword(QString user_password_)
{
    this->user_password = user_password_;
}

void Client::setRoomNum(quint16 room_number_)
{
    this->user_cur_room_number = room_number_;
}

void Client::setUserAvatarPath( QString path_)
{
    this->user_avatar_path = path_;
}

void Client::setUserRating(int rating_)
{
    this->user_rating = rating_;
}

