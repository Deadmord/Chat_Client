#pragma once

#include <QtWidgets/QMainWindow>
#include <QStandardItemModel>
#include <QTcpSocket>
#include <QInputDialog>
#include <QMessageBox>
#include <QHostAddress>
#include <QTime>
#include <QUuid>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFile>
#include "entities.h"
#include "client.h"
#include "ui_ChatClient.h"

const QString CONFIG_FILE_PATH = "./config.json";

QT_BEGIN_NAMESPACE
namespace Ui { class ChatClientUi; };
QT_END_NAMESPACE

class ChatClient : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(ChatClient)

public:
    explicit ChatClient(QWidget *parent = nullptr);
    ~ChatClient();
    void startClient();

public slots:
    void slotReadyRead();               //Old remove
    void slotDisconnect();              //Old remove

private:
    Message createMessage(QString nickame, QString text);
    void sendToServer(Message msg);     //Old remove
    void initConnection();              //Old remove
    //-------Config-file-functions-------
    void loadConfig(QString _path);
    void saveConfig(QString _path);
    void configFromJson(const QJsonDocument& config_json_);
    QJsonObject configToJson();

private slots:
    void attemptConnection();
    void connectedToServer();
    void attemptLogin(const QString& userName);
    void loggedIn();
    void loginFailed(const QString& reason);
    void messageReceived(const QString& sender, const QString& text);
    void sendMessage();
    void disconnectedFromServer();
    void userJoined(const QString& username);
    void userLeft(const QString& username);
    void errorSlot(QAbstractSocket::SocketError socketError);

private slots:
    void on_connectButton_clicked();    //Old remove
    void on_roomButton_clicked();       //Old remove
    void on_sendButton_clicked();       //Old remove
    void on_lineEdit_returnPressed();   //Old remove

    void on_nickNameLineEdit_returnPressed();   //Old remove
    void on_serverIPLineEdit_returnPressed();   //Old remove
    void on_serverPortLineEdit_returnPressed(); //Old remove
    void on_roomLineEdit_returnPressed();       //Old remove

private:
    Ui::ChatClientUi *ui;
    Client* client;
    QStandardItemModel* chat_model;
    QString last_user_name;

    QTcpSocket* socket;             //Old remove

    QByteArray Data;                //Old remove
    quint16 nextBlockSize = 0;      //Old remove

    QString server_address;         //Old remove
    quint16 server_port;            //Old remove
    quint16 flood_limit;            //Old remove
    QString msg_history_path;       //Old remove
};
