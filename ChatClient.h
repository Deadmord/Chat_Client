#pragma once

#include <QtWidgets/QMainWindow>
#include <QStandardItemModel>
#include <QTcpSocket>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QKeyEvent>
#include <QIcon>
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
#include "MessageItem.h"
#include "MessageWView.h"

const QString CONFIG_FILE_PATH = "./config.json";

QT_BEGIN_NAMESPACE
namespace Ui { class ChatClientClass; };
QT_END_NAMESPACE

class ChatClient : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(ChatClient)

public:
    explicit ChatClient(QWidget *parent = nullptr);
    ~ChatClient();

    ChatClient(const ChatClient&) = delete;
    ChatClient(ChatClient&&) = delete;
    const ChatClient& operator =(const ChatClient&) = delete;
    ChatClient& operator = (ChatClient&&) = delete;

    void startClient();

Q_SIGNALS:
    void new_message(const QVariant& msg);

public slots:
    void slotReadyRead();
    void slotDisconnect();

private:
    Message createMessage(QString nickame, QString text);

    //-------Config-file-functions-------
    void loadConfig(QString _path);
    void saveConfig(QString _path);
    void configFromJson(const QJsonDocument& config_json_);
    QJsonObject configToJson();

private Q_SLOTS:
    /*void on_sendButton_clicked();
    void on_attach_files();
    void on_image_clicked(const QString& image_path);*/

    void on_connectButton_clicked();
    void on_roomButton_clicked();
    void on_lineEdit_returnPressed();

    void on_nickNameLineEdit_returnPressed();
    void on_serverIPLineEdit_returnPressed();
    void on_serverPortLineEdit_returnPressed();
    void on_roomLineEdit_returnPressed();
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
    void keepCurrentConfig();

    //-----LogInW-----
    void on_log_in_button_clicked();
    void on_sign_in_button_clicked();

    //-----ChatListW-----

    //-----ChatW-----
    void on_sendButton_clicked();
    void on_attach_files();
    void on_image_clicked(const QString& image_path);

protected:

    void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::ChatClientClass *ui;
    QTcpSocket* socket;
    Client& client;
    QByteArray Data;
    //Ui::ChatClientUi* ui;
    //Client* client;
    QStandardItemModel* chat_model;
    QString last_user_name;

    QString server_address;         
    quint16 server_port;            
    quint16 flood_limit;            
    QString msg_history_path;       
};
