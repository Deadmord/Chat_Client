#include "ChatClient.h"

ChatClient::ChatClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatClientClass())
    , client(Client::instance())
{
    ui->setupUi(this);
}

ChatClient::~ChatClient()
{
    delete ui;
}

void ChatClient::startClient()
{
    loadConfig(CONFIG_FILE_PATH);                       //loading configuration settings
    //saveConfig(CONFIG_FILE_PATH);

    ui->serverIPLineEdit->setText(server_address);                  //������� � ������� uiInit()
    ui->serverPortLineEdit->setText(QString::number(server_port));
    ui->nickNameLineEdit->setText(client.getUserName());
    ui->passwordLineEdit->setText(client.getUserPassword());
    ui->roomLineEdit->setText(QString::number(client.getRoomNum()));

    initConnection();
}

void ChatClient::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if (in.status() == QDataStream::Ok)
    {
        //        QString str;
        //        in >> str;
        //        ui->textBrowser->append(str);
        for (;;)
        {
            //ToDo: looks like it sould be in separate thread, couse a waiting
            if (nextBlockSize == 0)
            {
                if (socket->bytesAvailable() < 2)
                {
                    break;
                }
                in >> nextBlockSize;
            }
            if (socket->bytesAvailable() < nextBlockSize)
            {
                break;
            }
            //strange conditionб warning
            Message msg;
            in >> msg.id >> msg.time >> msg.nickname >> msg.deleted >> msg.text;

            nextBlockSize = 0;
            if (!msg.deleted)                //TODO Create printmessage function
            {
                ui->textBrowser->append(msg.id + " " + msg.time.toString() + " " + msg.nickname + " :\t" + msg.text);
            }
        }
    }
    else
    {
        ui->textBrowser->append("Read error");
    }
}

void ChatClient::slotDisconnect()
{
    socket->deleteLater();
    ui->textBrowser->append("Server disconnected!");
}

void ChatClient::sendToServer(Message msg)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);

    out << quint16(0) << msg.id << msg.time << msg.nickname << msg.deleted << msg.text; // ToDo: define operators << and >> for "Messege"

    out.device()->seek(0);          //jamp to start block
    out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);
    ui->lineEdit->clear();
}
void ChatClient::initConnection()
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatClient::slotDisconnect);
}
Message ChatClient::createMessage(QString nickame, QString text)
{
    return Message{ nickame, text, QDateTime::currentDateTime(), QUuid::createUuid().toString(), false };
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

    if (const QJsonValue v = config_json["FloodLimit"]; v.isDouble())
        flood_limit = v.toInt();
    else
        qWarning() << "Error FloodLimit reading";

    if (const QJsonValue v = config_file_doc_["User"]["Nickname"]; v.isString())
        client.setUserName(v.toString());
    else
        qWarning() << "Error LastRoomNumber reading";

    if (const QJsonValue v = config_file_doc_["User"]["Password"]; v.isString())
        client.setUserPassword(v.toString());
    else
        qWarning() << "Error LastRoomNumber reading";

    if (const QJsonValue v = config_file_doc_["User"]["LastRoomNumber"]; v.isDouble())
        client.setRoomNum(v.toInt());
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
    json["FloodLimit"] = flood_limit;
    user["Nickname"] = client.getUserName();
    user["Password"] = client.getUserPassword();
    user["LastRoomNumber"] = client.getRoomNum();
    json["User"] = user;
    history["Path"] = msg_history_path;
    json["MessagesHistorySettings"] = history;

    return json;
}

//-------------window interface------------
void ChatClient::on_nickNameLineEdit_returnPressed()
{
    client.setUserName(ui->nickNameLineEdit->text());
}

void ChatClient::on_serverIPLineEdit_returnPressed()
{
    server_address = ui->serverIPLineEdit->text();
}

void ChatClient::on_serverPortLineEdit_returnPressed()
{
    server_port = ui->serverPortLineEdit->text().toUInt();
}

void ChatClient::on_roomLineEdit_returnPressed()
{
    client.setRoomNum(ui->roomLineEdit->text().toUInt());
}

void ChatClient::on_connectButton_clicked()
{
    server_address = (ui->serverIPLineEdit->text());            //вынести в отдельную функцию или слот
    server_port = (ui->serverPortLineEdit->text().toUInt());    //для чтения и записи config использовать структуру, где формировать обьект конфига
    client.setUserName(ui->nickNameLineEdit->text());
    client.setUserPassword(ui->passwordLineEdit->text());
    client.setRoomNum(ui->roomLineEdit->text().toUInt());

    saveConfig(CONFIG_FILE_PATH);

    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        if (client.getUserName().size() &&
            server_address.size() &&
            server_port)
        {
            socket->connectToHost(server_address, server_port);
            //with async await if(socket->state() == QAbstractSocket::ConnectedState)
            ui->connectButton->setText("Disconnect");
            //and only after that change button title
        }
        else
        {
            ui->textBrowser->append("Fill your name and server address, please.");
        }
    }
    else
    {
        socket->disconnectFromHost();
        ui->connectButton->setText("Connect");
        initConnection();
    }
}

void ChatClient::on_roomButton_clicked()
{

}

void ChatClient::on_sendButton_clicked()
{
    //Create message function
    //Send message function
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        sendToServer(createMessage(client.getUserName(), ui->lineEdit->text()));
    }
    else
    {
        ui->textBrowser->append("Not Connected to Server");
        qDebug() << socket->state();
    }
}
void ChatClient::on_lineEdit_returnPressed()
{
    on_sendButton_clicked();
}

