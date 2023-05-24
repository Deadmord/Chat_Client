#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTime>

class Client        //singleton
{
public:
    static Client& instance(QString userName = nullptr, QString hostName = nullptr, quint16 port = 0, quint16 roomNum = 0)
    {
        static Client inst(userName, hostName, port, roomNum);
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
    Client(QString &userName, QString &hostName, quint16 port, quint16 roomNum);
    ~Client();
    Client(Client const&) = delete;
    Client& operator= (Client const&) = delete;

private:
    QString userName = nullptr;

    QString hostName = nullptr;
    quint16 port = 0;
    quint16 roomNum = 0;

    QDateTime lastMessageTime;

};

#endif // CLIENT_H
