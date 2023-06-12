#ifndef CLIENT_H
#define CLIENT_H

#include <plog/Log.h> 
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
    const QString& getUserNickname();
    const QString& getUserPassword();
    quint16 getRoomNum();
    const QDateTime& getLastMessageTime();

    //---------setters-----------
    void setUserNickname(QString userName);
    void setUserPassword(QString hostName);
    void setRoomNum(quint16 roomNum);
    void setLastMessageTime();

signals:
    void connected();
    void disconnected();
    void loggedIn();
    void loginError(const QString& reason);
    void messageReceived(const QString& sender, const QString& text);
    void errorSignal(QAbstractSocket::SocketError socket_error);
    void userJoined(const QString& username);
    void userLeft(const QString& username);

public slots:
    void connectToServer(const QHostAddress& address, quint16 port);
    void login(const QString& username);
    void sendMessage(const QString& text);
    void disconnectFromHost();
private slots:
    void onReadyRead();

private:
    void initSocket();
    void jsonReceived(const QJsonObject& doc);

private:

    QTcpSocket* client_socket;
    QString user_nickname;
    QString user_password;
    quint16 user_current_chat_room_number;
    QPixmap user_picture_path;

    const QString initiate_picture_path = "/ images / avatar.png";
    bool logged_in;
    quint16 nextBlockSize = 0;  //the variable for keep size of reciving data
};

#endif // CLIENT_H
