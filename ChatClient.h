#pragma once

#include <QtWidgets/QMainWindow>
#include <QTcpSocket>
#include <QInputDialog>
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
#include <QList>
#include <QBuffer>

#include "entities.h"
#include "client.h"
#include "ui_ChatClient.h"
#include "MessageItem.h"
#include "MessageWView.h"

#include "ChatItem.h"
#include "ChatWView.h"
#include "DTOUser.h"

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
    void new_chat(const QVariant& chat_);
    void download_chat(const QVariantList& list_chats_);
    void download_messages(const QVariantList& list_msg_);
    void recivedLike(const QVariant& like_);


private Q_SLOTS:

    //-----CtartW-----
    void onStartAppClicked();

    //-----LogInW-----
    void on_log_in_button_clicked();
    void on_sign_in_button_clicked();

    //-----ProfileW
    void on_start_chatting_clicked();
    void onSaveClicked();
    void onChangeButtonAvatarClicked();
    void onChangeButtonPasswordClicked();

    //-----ChatListW-----
    void onAddChatButtonClicked();
    void onProfileClicked();

    //-----ChatW-----
    void on_sendButton_clicked();
    void on_attach_files();
    void on_image_clicked(const QString& image_path);
    void onChatClicked(qint32 chat_id_);
    void onReactionClick(const Likes& mes_user_likes_);


    //-----AddRoom-----
    void onCreateClicked();
    void onCancelClicked();

private slots:
    void attemptConnection();
    void connectedToServer();
    void loginFailed(const QString& reason);
    void loggedIn(const DTOUser& dto_user_);
    void createUserFailed(const QString& reason);

    void messageReceived(const DTOMessage& msg_);
    void messageListReceived(const QList<MessageItem>& list_of_mess);
    void roomCreated(const ChatItem& chat_);
    void topicsComes(const QStringList& topics_);
    void connectedToRoom(const QList<MessageItem>& list_of_mess);
    void userAvatarUpdated(const UserItem& user_);
    void userPasswordUpdated(const UserItem& user_);

    void likeReceivedServer(const Likes& like_);

    void disconnectedFromServer();

protected:

    void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::ChatClientClass *ui;
    Client* client;

    ConfigData config_data;
};
