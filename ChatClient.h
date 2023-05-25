#pragma once

#include <QtWidgets/QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QTime>
#include <QUuid>
#include "entities.h"
#include "client.h"
#include "ui_ChatClient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChatClientClass; };
QT_END_NAMESPACE

class ChatClient : public QMainWindow
{
    Q_OBJECT

public:
    ChatClient(Client& client, QWidget *parent = nullptr);
    ~ChatClient();

public slots:
    void slotReadyRead();
    void slotDisconnect();

private:
    Message createMessage(QString nickame, QString text);
    void sendToServer(Message msg);

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
    Ui::ChatClientClass *ui;
    QTcpSocket* socket;
    Client& client;
    QByteArray Data;
    quint16 nextBlockSize = 0;
};
