#pragma once

#include <QtWidgets/QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QTime>
#include <QUuid>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFile>
#include "entities.h"
#include "client.h"
#include "Message.h"

const QString CONFIG_FILE_PATH = "./config.json";

class ChatClient : public QMainWindow
{
    Q_OBJECT

public:
    ChatClient(QWidget *parent = nullptr);
    ~ChatClient();
    void startClient();

public slots:
    void slotReadyRead();
    void slotDisconnect();

private:
    MessageStruct createMessage(QString nickame, QString text);
    void sendToServer(MessageStruct msg);
    void initConnection();
    //-------Config-file-functions-------
    void loadConfig(QString _path);
    void saveConfig(QString _path);
    void configFromJson(const QJsonDocument& config_json_);
    QJsonObject configToJson();

private slots:
    void on_connectButton_clicked();
    void on_roomButton_clicked();
    void on_sendButton_clicked();
    void on_lineEdit_returnPressed();

    void on_nickNameLineEdit_returnPressed();
    void on_serverIPLineEdit_returnPressed();
    void on_serverPortLineEdit_returnPressed();
    void on_roomLineEdit_returnPressed();

private:
    Message mes_w;

    QTcpSocket* socket;
    Client& client;
    QByteArray Data;
    quint16 nextBlockSize = 0;

    QString server_address;         //
    quint16 server_port;            //
    quint16 flood_limit;            //
    QString msg_history_path;       //
};
