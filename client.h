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

class Client : public QObject        //singleton
{
    Q_OBJECT
    Q_DISABLE_COPY(Client)
private:
    Client(QObject* parent = nullptr);
    ~Client();
public:
    static Client* instance(QObject* parent = nullptr);

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
    void jsonReceived(const QJsonObject& doc);

private:
    QTcpSocket* client_socket;
    QString user_name;
    QString user_password;
    quint16 room_number;
    QDateTime last_message_time = {};
    bool logged_in;

};

#endif // CLIENT_H
