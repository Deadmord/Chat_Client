#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFile>

const QString CONFIG_FILE_PATH = "./config.json";

class Client        //singleton
{
public:
    static Client& instance(QString user_name_ = nullptr, QString server_address_ = nullptr, quint16 server_port_ = 0, quint16 room_number_ = 0)
    {
        static Client inst(user_name_, server_address_, server_port_, room_number_);
        return inst;
    };
//---------getters-----------
    const QString& getUserName();
    const QString& getHostName();
    quint16 getPort();
    quint16 getRoomNum();
    const QDateTime& getLastMessageTime();

    //---------setters-----------
    void setUserName(QString userName);
    void setHostName(QString hostName);
    void setPort(quint16 port);
    void setRoomNum(quint16 roomNum);
    void setLastMessageTime();

private:
    Client(QString user_name_, QString server_address_, quint16 server_port_, quint16 room_number_);
    ~Client();
    Client(Client const&) = delete;
    Client& operator= (Client const&) = delete;

    void loadConfig(QString _path);
    void saveConfig(QString _path);
    void configFromJson(const QJsonDocument& config_json_);
    QJsonObject configToJson();

private:
    QString user_name;
    QString user_password;

    QString server_address;         //move this var & relative func to ChatClient class
    quint16 server_port;            //
    quint16 flood_limit;            //
    quint16 room_number;            //
    QString msg_history_path;       //

    QDateTime last_message_time;
};

#endif // CLIENT_H
