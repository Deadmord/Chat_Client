#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTime>

class Client        //singleton
{
public:
    static Client& instance(QString user_name_ = nullptr, QString user_password_ = nullptr, quint16 room_number_ = 0)
    {
        static Client inst(user_name_, user_password_, room_number_);
        return inst;
    };
//---------getters-----------
    const QString& getUserName();
    const QString& getUserPassword();
    quint16 getRoomNum();
    const QDateTime& getLastMessageTime();

    //---------setters-----------
    void setUserName(QString userName);
    void setUserPassword(QString hostName);
    void setRoomNum(quint16 roomNum);
    void setLastMessageTime();

private:
    Client(QString user_name_, QString user_password_, quint16 room_number_);
    ~Client();
    Client(Client const&) = delete;
    Client& operator= (Client const&) = delete;

private:
    QString user_name;
    QString user_password;
    quint16 room_number;
    QDateTime last_message_time;
};

#endif // CLIENT_H
