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

private:
    Message createMessage(QString nickame, QString text);

    //-------Config-file-functions-------
    void loadConfig(QString _path);
    void saveConfig(QString _path);
    void configFromJson(const QJsonDocument& config_json_);
    QJsonObject configToJson();

private slots:
    void attemptConnection();
    void connectedToServer();
    void attemptLogin(const QString& userName, const QString& password);
    void loggedIn();
    void loginFailed(const QString& reason);
    void messageReceived(const QString& sender, const QString& text);
    void sendMessage();
    void disconnectedFromServer();
    void userJoined(const QString& username);
    void userLeft(const QString& username);
    void errorSlot(QAbstractSocket::SocketError socketError);
    void keepCurrentConfig();

private:
    Ui::ChatClientUi *ui;
    Client* client;
    QStandardItemModel* chat_model;
    QString last_user_name;

    QString server_address;         
    quint16 server_port;             
    QString msg_history_path;       
};
