#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTime>

#include <plog/Log.h> 

class Client        //singleton
{
public:
    static Client& instance(QString user_nickname_ = nullptr, QString user_password_ = nullptr, quint16 user_current_chat_room_number_ = 0)
    {
        static Client inst(user_nickname_, user_password_, user_current_chat_room_number_);
        return inst;
    };

    //---------getters-----------
    const QString& getUserNickname();
    const QString& getUserPassword();
    quint16 getRoomNum();
    const QDateTime& getLastMessageTime();

    //---------setters-----------
    void setUserNickname(QString userName);
    void setUserPassword(QString hostName);
    void setRoomNum(quint16 roomNum);
    void setLastMessageTime();

private:
    Client(QString user_nickname_, QString user_password_, quint16 user_current_chat_room_number_);
    ~Client();
    Client(Client const&) = delete;
    Client& operator= (Client const&) = delete;

private:
    QString user_nickname;
    QString user_password;
    quint16 user_current_chat_room_number;
    QDateTime last_message_time; //todo delelte
    QPixmap user_picture_path;

    const QString initiate_picture_path = "/ images / avatar.png";

};

#endif // CLIENT_H
