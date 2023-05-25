#include "client.h"

Client::Client(QString user_name_, QString server_address_, quint16 server_port_, quint16 room_number_)
    :user_name(user_name_), server_address(server_address_), server_port(server_port_), room_number(room_number_)
{
    last_message_time = QDateTime();
    loadConfig(CONFIG_FILE_PATH);
    saveConfig(CONFIG_FILE_PATH);
}

Client::~Client()
{

}

void Client::loadConfig(QString _path)
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

void Client::saveConfig(QString _path)
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
}

void Client::configFromJson(const QJsonDocument& config_file_doc_)
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

    if (const QJsonValue v = config_file_doc_["RoomsSettings"]["LastRoomNumber"]; v.isDouble())
        room_number = v.toInt();
    else
        qWarning() << "Error FloodLimit reading";

    if (const QJsonValue v = config_file_doc_["MessagesHistorySettings"]["Path"]; v.isString())
        msg_history_path = v.toString();
    else
        qWarning() << "Error MessagesHistorySettings path reading";
}

QJsonObject Client::configToJson()
{
    QJsonObject json;
    QJsonObject room;
    QJsonObject history;

    json["ServerAddress"] = server_address;
    json["ServerPort"] = server_port;
    json["FloodLimit"] = flood_limit;
    room["LastRoomNumber"] = room_number;
    json["RoomsSettings"] = room;
    history["Path"] = msg_history_path;
    json["MessagesHistorySettings"] = history;

    return json;
}

//------------getters-----------
const QString& Client::getUserName() {return user_name;}

const QString &Client::getHostName() {return server_address;}

quint16 Client::getPort() {return server_port;}

quint16 Client::getRoomNum() {return room_number;}

const QDateTime &Client::getLastMessageTime() {return last_message_time;}

//------------setters-----------
void Client::setUserName(QString user_name_)
{
    this->user_name = user_name_;
}

void Client::setHostName(QString server_address_)
{
    this->server_address = server_address_;
}

void Client::setPort(quint16 server_port_)
{
    this->server_port = server_port_;
}

void Client::setRoomNum(quint16 room_number_)
{
    this->room_number = room_number_;
}

void Client::setLastMessageTime()
{
    this->last_message_time = QDateTime::currentDateTime();
}

