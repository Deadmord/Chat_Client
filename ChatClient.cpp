#include "ChatClient.h"

ChatClient::ChatClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatClientUi())    // create the elements defined in the .ui file
    , client(Client::instance())    // create the chat client
    , chat_model(new QStandardItemModel(this)) // create the model to hold the messages
{
    ui->setupUi(this);
    // the model for the messages will have 1 column
    chat_model->insertColumn(0);
    // set the model as the data source vor the list view
    ui->chatView->setModel(chat_model);
    // connect the signals from the chat client to the slots in this ui
    connect(client, &Client::connected, this, &ChatClient::connectedToServer);
    connect(client, &Client::loggedIn, this, &ChatClient::loggedIn);
    connect(client, &Client::loginError, this, &ChatClient::loginFailed);
    connect(client, &Client::messageReceived, this, &ChatClient::messageReceived);
    connect(client, &Client::disconnected, this, &ChatClient::disconnectedFromServer);
    connect(client, &Client::errorSignal, this, &ChatClient::errorSlot);
    connect(client, &Client::userJoined, this, &ChatClient::userJoined);
    connect(client, &Client::userLeft, this, &ChatClient::userLeft);
    // connect the connect button to a slot that will attempt the connection
    connect(ui->connectButton, &QPushButton::clicked, this, &ChatClient::attemptConnection);
    connect(ui->connectButton, &QPushButton::clicked, this, &ChatClient::keepCurrentConfig);
    // connect the click of the "send" button and the press of the enter while typing to the slot that sends the message
    connect(ui->sendButton, &QPushButton::clicked, this, &ChatClient::sendMessage);
    connect(ui->messageEdit, &QLineEdit::returnPressed, this, &ChatClient::sendMessage);
}

ChatClient::~ChatClient()
{
    // delete the elements created from the .ui file
    delete ui;
}

void ChatClient::startClient()
{
    loadConfig(CONFIG_FILE_PATH);                       //loading configuration settings
    //saveConfig(CONFIG_FILE_PATH);

    ui->serverIPLineEdit->setText(server_address);                  //вынести в функцию uiInit()
    ui->serverPortLineEdit->setText(QString::number(server_port));
    ui->nickNameLineEdit->setText(client->getUserName());
    ui->passwordLineEdit->setText(client->getUserPassword());
    ui->roomLineEdit->setText(QString::number(client->getRoomNum()));
}

void ChatClient::loadConfig(QString _path)
{
    QFile config_file;
    QJsonDocument config_file_doc;
    QJsonObject config_json;
    QJsonParseError jsonError;

    config_file.setFileName(_path);

    if (config_file.open(QIODevice::ReadOnly | QFile::Text))
    {
        config_file_doc = QJsonDocument::fromJson(QByteArray(config_file.readAll()), &jsonError);
        config_file.close();

        if (jsonError.error == QJsonParseError::NoError)
        {
            configFromJson(config_file_doc);
        }
        else
        {
            qWarning() << "Error config file read: " << jsonError.error;
        }
    }
    else
    {
        qWarning("Error configuration file cannot be opened.");
    }
}

void ChatClient::saveConfig(QString _path)
{
    QFile config_file;
    QJsonDocument config_file_doc;
    QJsonObject config_json;
    QJsonParseError jsonError;

    config_file.setFileName(_path);
    if (config_file.open(QIODevice::WriteOnly | QFile::Text))
    {
        config_json = configToJson();
        config_file.write(QJsonDocument(config_json).toJson());
    }
    else
    {
        qDebug() << "Error configuration file cannot be opened.";
    }
    config_file.close();
}

void ChatClient::configFromJson(const QJsonDocument& config_file_doc_)
{
    QJsonObject config_json = config_file_doc_.object();

    if (const QJsonValue v = config_json["ServerAddress"]; v.isString())
        server_address = v.toString();
    else
        qWarning() << "Error ServerAddress reading";

    if (const QJsonValue v = config_json["ServerPort"]; v.isDouble())
        server_port = v.toInt();
    else
        qWarning() << "Error ServerPort reading";

    if (const QJsonValue v = config_file_doc_["User"]["Nickname"]; v.isString())
        client->setUserName(v.toString());
    else
        qWarning() << "Error LastRoomNumber reading";

    if (const QJsonValue v = config_file_doc_["User"]["Password"]; v.isString())
        client->setUserPassword(v.toString());
    else
        qWarning() << "Error LastRoomNumber reading";

    if (const QJsonValue v = config_file_doc_["User"]["LastRoomNumber"]; v.isDouble())
        client->setRoomNum(v.toInt());
    else
        qWarning() << "Error LastRoomNumber reading";

    if (const QJsonValue v = config_file_doc_["MessagesHistorySettings"]["Path"]; v.isString())
        msg_history_path = v.toString();
    else
        qWarning() << "Error MessagesHistorySettings path reading";
}

QJsonObject ChatClient::configToJson()
{
    QJsonObject json;
    QJsonObject user;
    QJsonObject history;

    json["ServerAddress"] = server_address;
    json["ServerPort"] = server_port;
    user["Nickname"] = client->getUserName();
    user["Password"] = client->getUserPassword();
    user["LastRoomNumber"] = client->getRoomNum();
    json["User"] = user;
    history["Path"] = msg_history_path;
    json["MessagesHistorySettings"] = history;

    return json;
}

void ChatClient::keepCurrentConfig()
{
    server_address = (ui->serverIPLineEdit->text());            //вынести в отдельную функцию или слот
    server_port = (ui->serverPortLineEdit->text().toUInt());    //для чтения и записи config использовать структуру, где формировать обьект конфига
    client->setUserName(ui->nickNameLineEdit->text());
    client->setUserPassword(ui->passwordLineEdit->text());
    client->setRoomNum(ui->roomLineEdit->text().toUInt());

    saveConfig(CONFIG_FILE_PATH);
}
//-----Old start------------------------
//void ChatClient::slotReadyRead()
//{
//    QDataStream in(socket);
//    in.setVersion(QDataStream::Qt_6_2);
//    if (in.status() == QDataStream::Ok)
//    {
//        //        QString str;
//        //        in >> str;
//        //        ui->textBrowser->append(str);
//        for (;;)
//        {
//            //ToDo: looks like it sould be in separate thread, couse a waiting
//            if (nextBlockSize == 0)
//            {
//                if (socket->bytesAvailable() < 2)
//                {
//                    break;
//                }
//                in >> nextBlockSize;
//            }
//            if (socket->bytesAvailable() < nextBlockSize)
//            {
//                break;
//            }
//            //strange conditionб warning
//            Message msg;
//            in >> msg.id >> msg.time >> msg.nickname >> msg.deleted >> msg.text;
//
//            nextBlockSize = 0;
//            if (!msg.deleted)                //TODO Create printmessage function
//            {
//                //ui->textBrowser->append(msg.id + " " + msg.time.toString() + " " + msg.nickname + " :\t" + msg.text);
//            }
//        }
//    }
//    else
//    {
//        //ui->textBrowser->append("Read error");
//    }
//}


//void ChatClient::sendToServer(Message msg)
//{
//    Data.clear();
//    QDataStream out(&Data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_6_2);
//
//    out << quint16(0) << msg.id << msg.time << msg.nickname << msg.deleted << msg.text; // ToDo: define operators << and >> for "Messege"
//
//    out.device()->seek(0);          //jamp to start block
//    out << quint16(Data.size() - sizeof(quint16));
//    socket->write(Data);
//    ui->messageEdit->clear();
//}

//Message ChatClient::createMessage(QString nickame, QString text)
//{
//    return Message{ nickame, text, QDateTime::currentDateTime(), QUuid::createUuid().toString(), false };
//}

//-----Old finish------------------------

//-----New start-------------------------
void ChatClient::attemptConnection()
{
    if (client->socketInfo()->state() == QAbstractSocket::UnconnectedState)
    {
        // We ask the user for the address of the server, we use 127.0.0.1 (aka localhost) as default
        const QString hostAddress = QInputDialog::getText(
            this
            , tr("Chose Server")
            , tr("Server Address")
            , QLineEdit::Normal
            , server_address
        );
        if (hostAddress.isEmpty())
            return; // the user pressed cancel or typed nothing
        // disable the connect button to prevent the user clicking it again
        // tell the client to connect to the host using the port 5555
        client->connectToServer(QHostAddress(hostAddress), server_port);
    }
    else
    {
        client->disconnectFromHost();
        //initConnection(); Снова связать рэдирид и делитпотом
    }
}

void ChatClient::connectedToServer()
{
    ui->connectButton->setText("Disconnect");
    // once we connected to the server we ask the user for what username they would like to use
    const QString newUsername = QInputDialog::getText(this, tr("Chose Username"), tr("Username"), QLineEdit::Normal, client->getUserName());
    if (newUsername.isEmpty()) {
        // if the user clicked cancel or typed nothing, we just disconnect from the server
        return client->disconnectFromHost();
    }
    // try to login with the given username
    attemptLogin(newUsername);
}

void ChatClient::attemptLogin(const QString& userName)
{
    // use the client to attempt a log in with the given username
    client->login(userName);
}

void ChatClient::loggedIn()
{
    // once we successully log in we enable the ui to display and send messages
    ui->sendButton->setEnabled(true);
    ui->messageEdit->setEnabled(true);
    ui->chatView->setEnabled(true);
    // clear the user name record
    last_user_name.clear();
}

void ChatClient::loginFailed(const QString& reason)
{
    // the server rejected the login attempt
    // display the reason for the rejection in a message box
    QMessageBox::critical(this, tr("Error"), reason);
    // allow the user to retry, execute the same slot as when just connected
    connectedToServer();
}

void ChatClient::messageReceived(const QString& sender, const QString& text)
{
    // store the index of the new row to append to the model containing the messages
    int newRow = chat_model->rowCount();
    // we display a line containing the username only if it's different from the last username we displayed
    if (last_user_name != sender) {
        // store the last displayed username
        last_user_name = sender;
        // create a bold default font
        QFont boldFont;
        boldFont.setBold(true);
        // insert 2 row, one for the message and one for the username
        chat_model->insertRows(newRow, 2);
        // store the username in the model
        chat_model->setData(chat_model->index(newRow, 0), sender + QLatin1Char(':'));
        // set the alignment for the username
        chat_model->setData(chat_model->index(newRow, 0), int(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextAlignmentRole);
        // set the for the username
        chat_model->setData(chat_model->index(newRow, 0), boldFont, Qt::FontRole);
        ++newRow;
    }
    else {
        // insert a row for the message
        chat_model->insertRow(newRow);
    }
    // store the message in the model
    chat_model->setData(chat_model->index(newRow, 0), text);
    // set the alignment for the message
    chat_model->setData(chat_model->index(newRow, 0), int(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextAlignmentRole);
    // scroll the view to display the new message
    ui->chatView->scrollToBottom();
}

void ChatClient::sendMessage()
{
    if (client->socketInfo()->state() == QAbstractSocket::ConnectedState)
    {
        // we use the client to send the message that the user typed
        client->sendMessage(ui->messageEdit->text());
        // now we add the message to the list
        // store the index of the new row to append to the model containing the messages
        const int newRow = chat_model->rowCount();
        // insert a row for the message
        chat_model->insertRow(newRow);
        // store the message in the model
        chat_model->setData(chat_model->index(newRow, 0), ui->messageEdit->text());
        // set the alignment for the message
        chat_model->setData(chat_model->index(newRow, 0), int(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
        // clear the content of the message editor
        ui->messageEdit->clear();
        // scroll the view to display the new message
        ui->chatView->scrollToBottom();
        // reset the last printed username
        last_user_name.clear();
    }
    else
    {
        const int newRow = chat_model->rowCount();
        chat_model->insertRow(newRow);
        chat_model->setData(chat_model->index(newRow, 0), "You are disconnected. Try reconnect, please...");
        chat_model->setData(chat_model->index(newRow, 0), int(Qt::AlignCenter | Qt::AlignVCenter), Qt::TextAlignmentRole);
        ui->chatView->scrollToBottom();
        last_user_name.clear();
    }
}

void ChatClient::disconnectedFromServer()
{
    ui->connectButton->setText("Connect");
    // if the client loses connection to the server
    // comunicate the event to the user via a message box
    QMessageBox::warning(this, tr("Disconnected"), tr("The host terminated the connection"));
    // disable the ui to send and display messages
    ui->sendButton->setEnabled(false);
    ui->messageEdit->setEnabled(false);
    ui->chatView->setEnabled(false);
    // enable the button to connect to the server again
    ui->connectButton->setEnabled(true);
    // reset the last printed username
    last_user_name.clear();
}

void ChatClient::userJoined(const QString& username)
{
    // store the index of the new row to append to the model containing the messages
    const int newRow = chat_model->rowCount();
    // insert a row
    chat_model->insertRow(newRow);
    // store in the model the message to comunicate a user joined
    chat_model->setData(chat_model->index(newRow, 0), tr("%1 Joined the Chat").arg(username));
    // set the alignment for the text
    chat_model->setData(chat_model->index(newRow, 0), Qt::AlignCenter, Qt::TextAlignmentRole);
    // set the color for the text
    chat_model->setData(chat_model->index(newRow, 0), QBrush(Qt::blue), Qt::ForegroundRole);
    // scroll the view to display the new message
    ui->chatView->scrollToBottom();
    // reset the last printed username
    last_user_name.clear();
}
void ChatClient::userLeft(const QString& username)
{
    // store the index of the new row to append to the model containing the messages
    const int newRow = chat_model->rowCount();
    // insert a row
    chat_model->insertRow(newRow);
    // store in the model the message to comunicate a user left
    chat_model->setData(chat_model->index(newRow, 0), tr("%1 Left the Chat").arg(username));
    // set the alignment for the text
    chat_model->setData(chat_model->index(newRow, 0), Qt::AlignCenter, Qt::TextAlignmentRole);
    // set the color for the text
    chat_model->setData(chat_model->index(newRow, 0), QBrush(Qt::red), Qt::ForegroundRole);
    // scroll the view to display the new message
    ui->chatView->scrollToBottom();
    // reset the last printed username
    last_user_name.clear();
}

void ChatClient::errorSlot(QAbstractSocket::SocketError socketError)
{
    // show a message to the user that informs of what kind of error occurred
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
    case QAbstractSocket::ProxyConnectionClosedError:
        return; // handled by disconnectedFromServer
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::critical(this, tr("Error"), tr("The host refused the connection"));
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        QMessageBox::critical(this, tr("Error"), tr("The proxy refused the connection"));
        break;
    case QAbstractSocket::ProxyNotFoundError:
        QMessageBox::critical(this, tr("Error"), tr("Could not find the proxy"));
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::critical(this, tr("Error"), tr("Could not find the server"));
        break;
    case QAbstractSocket::SocketAccessError:
        QMessageBox::critical(this, tr("Error"), tr("You don't have permissions to execute this operation"));
        break;
    case QAbstractSocket::SocketResourceError:
        QMessageBox::critical(this, tr("Error"), tr("Too many connections opened"));
        break;
    case QAbstractSocket::SocketTimeoutError:
        QMessageBox::warning(this, tr("Error"), tr("Operation timed out"));
        return;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        QMessageBox::critical(this, tr("Error"), tr("Proxy timed out"));
        break;
    case QAbstractSocket::NetworkError:
        QMessageBox::critical(this, tr("Error"), tr("Unable to reach the network"));
        break;
    case QAbstractSocket::UnknownSocketError:
        QMessageBox::critical(this, tr("Error"), tr("An unknown error occured"));
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        QMessageBox::critical(this, tr("Error"), tr("Operation not supported"));
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        QMessageBox::critical(this, tr("Error"), tr("Your proxy requires authentication"));
        break;
    case QAbstractSocket::ProxyProtocolError:
        QMessageBox::critical(this, tr("Error"), tr("Proxy comunication failed"));
        break;
    case QAbstractSocket::TemporaryError:
    case QAbstractSocket::OperationError:
        QMessageBox::warning(this, tr("Error"), tr("Operation failed, please try again"));
        return;
    default:
        Q_UNREACHABLE();
    }
    // enable the button to connect to the server again
    ui->connectButton->setEnabled(true);
    // disable the ui to send and display messages
    ui->sendButton->setEnabled(false);
    ui->messageEdit->setEnabled(false);
    ui->chatView->setEnabled(false);
    // reset the last printed username
    last_user_name.clear();
}
