#pragma once

#include <QtWidgets/QMainWindow>
#include <QTcpSocket>
//#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QKeyEvent>
#include <QIcon>
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

#include "ChatItem.h"
#include "ChatWView.h"

#include "ConfigService.h"

const QString CONFIG_FILE_PATH = "./config.json";

QT_BEGIN_NAMESPACE
namespace Ui { class ChatClientClass; };
QT_END_NAMESPACE

class ChatClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatClient(QWidget *parent = nullptr);
    ~ChatClient();

    ChatClient(const ChatClient&) = delete;
    ChatClient(ChatClient&&) = delete;
    const ChatClient& operator =(const ChatClient&) = delete;
    ChatClient& operator = (ChatClient&&) = delete;

Q_SIGNALS:
    void new_message(const QVariant& msg);
    void new_chat(const QVariant& msg);

private Q_SLOTS:

    //-----CtartW-----
    void onStartAppClicked();

    //-----LogInW-----
    void on_log_in_button_clicked();
    void on_sign_in_button_clicked();

    //-----ProfileW
    void on_start_chatting_clicked();

    //-----ChatListW-----
    void onAddChatButtonClicked();

    //-----ChatW-----
    void on_sendButton_clicked();
    void on_attach_files();
    void on_image_clicked(const QString& image_path);

private slots:
    void attemptConnection();
    void connectedToServer();
    void loginFailed(const QString& reason);
    void loggedIn();
    void messageReceived(const MessageItem& msg_);


    void disconnectedFromServer();

protected:

    void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::ChatClientClass *ui;
    Client* client;

    ConfigData config_data;
};
