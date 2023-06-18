#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
#include <QTime>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonParseError>

#include "MessageItem.h"

class Client : public QObject        //singleton
{
    Q_OBJECT
    Q_DISABLE_COPY(Client)
private:
    Client(QObject* parent = nullptr);
    ~Client();
public:
    static Client* instance(QObject* parent = nullptr);
    const QTcpSocket* socketInfo();
    //---------getters-----------
    const QString&  getUserNickname();
    const QString&  getUserPassword();
    quint16         getRoomNum();
    const QString&  getUserAvatarPath();
    int             getUserRating();

    //---------setters-----------
    void setUserName(QString userName);
    void setUserPassword(QString hostName);
    void setRoomNum(quint16 roomNum);
    void setUserAvatarPath(QString path_);
    void setUserRating(int raiting_);

signals:
    void connected();
    void disconnected();
    void loggedIn();
    void loginError(const QString& reason);
    void messageReceived(const MessageItem& msg_);
    void errorSignal(QAbstractSocket::SocketError socket_error);
    void userJoined(const QString& username);
    void userLeft(const QString& username);

public slots:
    void connectToServer(const QHostAddress& address, quint16 port);
    void login(const QString& userNickname_, const QString& userPassword_);
    void roomListRequest();
    void entryRoom(quint16 room_number_);
    void sendMessage(const QString& text);
    void disconnectFromHost();
private slots:
    void onReadyRead();

private:
    void initSocket();
    void jsonReceived(const QJsonObject& doc);
    void sendJson(const QJsonObject& doc);

private:
    QTcpSocket*     client_socket;

    QString         user_nickname;
    QString         user_password;
    quint16         user_cur_room_number = 0;
    QString         user_avatar_path = "./images/avatar.png";
    int             user_rating = 0;
    bool            logged_in;
    quint16         nextBlockSize = 0;  //the variable for keep size of reciving data
};

#endif // CLIENT_H