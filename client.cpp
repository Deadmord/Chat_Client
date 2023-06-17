#include "client.h"

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

void Client::login(const QString& userName, const QString& password)
{
    if (client_socket->state() == QAbstractSocket::ConnectedState) { // if the client is connected
        QByteArray buffer;
        buffer.clear();
        // create a QDataStream for buffer operating 
        QDataStream clientStream(&buffer, QIODevice::WriteOnly);
        // set the version so that programs compiled with different versions of Qt can agree on how to serialise
        clientStream.setVersion(QDataStream::Qt_6_5);
        // Create the JSON we want to send
        QJsonObject message;
        message[QStringLiteral("type")] = QStringLiteral("login");
        message[QStringLiteral("username")] = userName;
        message[QStringLiteral("password")] = password;
        // send the JSON using QDataStream
        const QByteArray jsonData = QJsonDocument(message).toJson(QJsonDocument::Compact);
        clientStream << quint16(0) << jsonData;
        clientStream.device()->seek(0); //go to beginning data storage
        clientStream << quint16(buffer.size() - sizeof(quint16));
        client_socket->write(buffer);
    }
}

void Client::sendMessage(const QString& text)
{
    if (text.isEmpty())
        return; // We don't send empty messages
    QByteArray buffer;
    buffer.clear();
    // create a QDataStream for buffer operating 
    QDataStream clientStream(&buffer, QIODevice::WriteOnly);
    // set the version so that programs compiled with different versions of Qt can agree on how to serialise
    clientStream.setVersion(QDataStream::Qt_6_5);
    // Create the JSON we want to send
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("message");
    message[QStringLiteral("text")] = text;
    // reserv size part in stream and send the JSON using QDataStream
    const QByteArray jsonData = QJsonDocument(message).toJson();
    clientStream << quint16(0) << jsonData; 
    clientStream.device()->seek(0); //go to beginning data storage
    clientStream << quint16(buffer.size() - sizeof(quint16));
    client_socket->write(buffer);
}

void Client::disconnectFromHost()
{
    client_socket->disconnectFromHost();
    initSocket();
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
            emit loggedIn();
            return;
        }
        // the login attempt failed, we extract the reason of the failure from the JSON
        // and notify it via the loginError signal
        const QJsonValue reasonVal = docObj.value(QLatin1String("reason"));
        emit loginError(reasonVal.toString());
    }
    else if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) == 0) { //It's a chat message
        // we extract the text field containing the chat text
        const QJsonValue textVal = docObj.value(QLatin1String("text"));
        // we extract the sender field containing the username of the sender
        const QJsonValue senderVal = docObj.value(QLatin1String("sender"));
        if (textVal.isNull() || !textVal.isString())
            return; // the text field was invalid so we ignore
        if (senderVal.isNull() || !senderVal.isString())
            return; // the sender field was invalid so we ignore
        // we notify a new message was received via the messageReceived signal
        emit messageReceived(senderVal.toString(), textVal.toString());
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
}

void Client::connectToServer(const QHostAddress& address, quint16 port)
{
    client_socket->connectToHost(address, port);
}

void Client::onReadyRead()
{
    // prepare a container to hold the UTF-8 encoded JSON we receive from the socket
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
const QString& Client::getUserName() {return user_name;}

const QString &Client::getUserPassword() {return user_password;}

quint16 Client::getRoomNum() {return room_number;}

const QDateTime &Client::getLastMessageTime() {return last_message_time;}

//------------setters-----------
void Client::setUserName(QString user_name_)
{
    this->user_name = user_name_;
}

void Client::setUserPassword(QString user_password_)
{
    this->user_password = user_password_;
}

void Client::setRoomNum(quint16 room_number_)
{
    this->room_number = room_number_;
}

void Client::setLastMessageTime()
{
    this->last_message_time = QDateTime::currentDateTime();
}

