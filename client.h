#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTime>

class Client        //singleton
{
public:
    static Client& instance(QString user_name_ = nullptr, QString user_password_ = nullptr, quint16 room_curr_number_ = 0)
    {
        static Client inst(user_name_, user_password_, room_curr_number_);
        return inst;
    };
//---------getters-----------
    const QString& getUserNickname();
    const QString& getUserPassword();
    quint16 getRoomNum();
    const QDateTime& getLastMessageTime();

    //---------setters-----------
    void setUserNickname(QString user_nickname_);
    void setUserPassword(QString user_password_);
    void setRoomNum(quint16 room_curr_number_);
    void setLastMessageTime();

private:
    Client(QString user_name_, QString user_password_, quint16 room_number_);
    ~Client();
    Client(Client const&) = delete;
    Client& operator= (Client const&) = delete;

private:
    QString user_nickname;
    QString user_password;
    quint16 user_curr_room_number;
};

#endif // CLIENT_H
