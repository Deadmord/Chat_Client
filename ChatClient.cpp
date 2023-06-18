#include "ChatClient.h"

#if defined (Q_OS_WIN) // Only valid if Qt was configured as a static build!
#include <Windows.h>

bool IsCurrentInputLanguageRTL(void)
{
    bool ret = false;
    auto layout = GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), NULL));

    auto lcid = MAKELCID(LOWORD(layout), SORT_DEFAULT);
    LOCALESIGNATURE localesig;

    // Windows XP and higher.
    // Unicode subset bit fields: https://msdn.microsoft.com/en-us/library/windows/desktop/dd374090(v=vs.85).aspx
    // Bit 123: Windows 2000 and later - Layout progress, horizontal from right to left.
    if (GetLocaleInfoW(lcid, LOCALE_FONTSIGNATURE, (LPWSTR)&localesig, sizeof(localesig) / sizeof(WCHAR)) != 0)
        ret = (localesig.lsUsb[3] & 0x08000000) != 0;

    return ret;
}
#endif

ChatClient::ChatClient(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatClientClass())
    , client(Client::instance())
    , config_data(ConfigData::getConfig())
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    //ui->stackedWidget->setCurrentIndex(3); //Just for test

    //Connections with view
    connect(this, &ChatClient::new_message, ui->chat_listView, &MessageWView::onMessageAdded);
    connect(this, &ChatClient::download_messages, ui->chat_listView, &MessageWView::onMessagesAdded);
    connect(this, &ChatClient::recivedLike, ui->chat_listView, &MessageWView::onRecivedLike);


    connect(this, &ChatClient::new_chat, ui->chatList_listView, &ChatWView::onChatAdded);
    connect(this, &ChatClient::download_chat, ui->chatList_listView, &ChatWView::onChatsAdded);

    connect(ui->chat_listView, &MessageWView::imageClicked, this, &ChatClient::on_image_clicked);
    connect(ui->chatList_listView, &ChatWView::chatClicked, this, &ChatClient::onChatClicked);
    connect(ui->chat_listView, &MessageWView::makeUserReaction, this, &ChatClient::onReactionClick);

    //connect(ui.plainTextEdit, &QPlainTextEdit::textChanged, this, &ChatClient::on_message_text_changed);
    //StartW
    connect(ui->start_app_button, &QPushButton::clicked, this, &ChatClient::onStartAppClicked);

    //LoginW
    connect(ui->login_sign_in_button, &QPushButton::clicked, this, &ChatClient::on_sign_in_button_clicked);
    connect(ui->login_log_in_button, &QPushButton::clicked, this, &ChatClient::on_log_in_button_clicked);

    //ProfileW
    connect(ui->profile_start_chating_button, &QPushButton::clicked, this, &ChatClient::on_start_chatting_clicked);

    //ChatListW
    connect(ui->chatList_add_chat_button, &QPushButton::clicked, this, &ChatClient::onAddChatButtonClicked);
    connect(ui->chatList_profile_button, &QPushButton::clicked, this, &ChatClient::onProfileClicked);

    //ChatRoomW
    connect(ui->send_button, &QPushButton::clicked, this, &ChatClient::on_sendButton_clicked);
    connect(ui->add_attach_button, &QPushButton::clicked, this, &ChatClient::on_attach_files);

    //AddRoom
    connect(ui->add_room_cancel_button, &QPushButton::clicked, this, &ChatClient::onCancelClicked);
    connect(ui->add_room_create_button, &QPushButton::clicked, this, &ChatClient::onCreateClicked);


    //Connections with client
    connect(client, &Client::connected, this, &ChatClient::connectedToServer);
    connect(client, &Client::loginError, this, &ChatClient::loginFailed);
    connect(client, &Client::loggedIn, this, &ChatClient::loggedIn);
    connect(client, &Client::messageReceived, this, &ChatClient::messageReceived);
    //connect(client, &Client::roomCreated, this, &ChatClient::roomCreated); //TODO MAKE in client

    connect(client, &Client::disconnected, this, &ChatClient::disconnectedFromServer); //TODO make a buttton to disconnect

}

ChatClient::~ChatClient()
{
    delete ui;
}

//-------------------------------BUTTONS-------------------------------
//-----Start Page
void ChatClient::onStartAppClicked() {
    attemptConnection();
}

//-----LogIn Page
void ChatClient::on_log_in_button_clicked() 
{
    if (!ui->login_nickname_edit->text().isEmpty() && !ui->login_password_edit->text().isEmpty()) {
        client->login(ui->login_nickname_edit->text(), ui->login_password_edit->text());

        //TODO delete. Just for time
        loggedIn({ "Niko",3, {} });
    }
    else
        QMessageBox::information(this, "Warning", "Please input all fields");
}

void ChatClient::on_sign_in_button_clicked()
{
    ui->profile_edit_save_button->setText("Save");
    ui->profile_start_chating_button->setEnabled(false);
    QPixmap pixmap("./images/avatar.png");

    // Set the pixmap to the QLabel
    ui->profile_image_lable->setPixmap(pixmap);
    ui->stackedWidget->setCurrentIndex(2);
}

//-----Profile Page

void ChatClient::on_start_chatting_clicked() {
    ui->text_edit->setPlaceholderText("Enter message text here");
    ui->stackedWidget->setCurrentIndex(3);

    auto listik = QVariantList{
        QVariant::fromValue<chatItemPtr>
        (
            chatItemPtr{ new ChatItem(
                1
                , "Name_1"
                , "Description_1"
                , "Lable_1"
                , false)
            }
        ),
        QVariant::fromValue<chatItemPtr>
        (
            chatItemPtr{ new ChatItem(
                2
                , "Name_2"
                , "Description_2"
                , "Lable_2"
                , false)
            }
        )
    };

    Q_EMIT download_chat(listik);
}

void ChatClient::onSaveEditClicked() {
    if (ui->profile_edit_save_button->text() == "Save") {
        //TODO send to client changed info
    }
    else {
        //TODO send to client info about new user
    }
}

//-----ChatList Page

void ChatClient::onChatClicked(qint32 chat_id_)
{
client->roomListRequest();
    //client->setRoomNum(chat_id_);
    //TODO send to server new current room

    //TODO Delete
    auto listik = QVariantList{
        QVariant::fromValue<messageItemPtr>
        (
            messageItemPtr{ new MessageItem(
                "1"
                ,"Lisa"
                , "Hello"
                , false
                , {})
            }
        ),
        QVariant::fromValue<messageItemPtr>
        (
            messageItemPtr{ new MessageItem(
                "2"
                , "Anton"
                , "Hello"
                , false
                , {})
            }
        ),
        QVariant::fromValue<messageItemPtr>
        (
            messageItemPtr{ new MessageItem(
                "3"
                , "Anton"
                , "םדדם"
                , true
                , {})
            }
        )
    };

    Q_EMIT download_messages(listik);

    ui->stackedWidget->setCurrentIndex(4);
};

void ChatClient::onAddChatButtonClicked()
{

    ui->stackedWidget->setCurrentIndex(3);
    ui->add_room_name_edit->clear();
    ui->add_room_descr_edit->clear();
    ui->add_room_combo_box->clear();
    ui->add_room_private_check_box->setCheckState(Qt::Unchecked);
    ui->add_room_password_edit->clear();

    //TODO ask a client for list of current topics
    //TODO delete. Just for time
    ui->add_room_combo_box->addItem("test");
    ui->stackedWidget->setCurrentIndex(5);
}

void ChatClient::onProfileClicked()
{
    //TODO ask from Server data about user (in client emit loggedIn)
};

//-----Chat Create Page

void ChatClient::onCreateClicked()
{
    if (!ui->add_room_name_edit->text().isEmpty() && !ui->add_room_descr_edit->toPlainText().isEmpty() && !ui->add_room_combo_box->currentText().isEmpty()) {
        //TODO send a dtat for server

        //TODO delete. Just for time
        ui->stackedWidget->setCurrentIndex(3);
    }
    else
        QMessageBox::information(this, "Warning", "Please input all fields");

    //todo call client, send a room
    Q_EMIT new_chat(
        QVariant::fromValue<chatItemPtr>
        (
            chatItemPtr{ new ChatItem(
                1
                , ui->add_room_name_edit->text()
                , ui->add_room_descr_edit->toPlainText()
                , ui->add_room_combo_box->currentText()
                , ui->add_room_private_check_box->isTristate()
                , ui->add_room_password_edit->text()) }
        )
    );

    //TODO wrapper for checkBOX + password. Disables checkbox - no password

};

void ChatClient::onCancelClicked()
{
    ui->stackedWidget->setCurrentIndex(3);
};

//-----MessageList Page

void ChatClient::on_sendButton_clicked()
{
    if (ui->text_edit->toPlainText().isEmpty())
        return;
    if (!ui->send_button->isEnabled())
        return;

    static auto ID = 0ULL;
    ++ID;

    const auto nickname = "Anton";
    const auto text = ui->text_edit->toPlainText();
    auto const is_RTL = IsCurrentInputLanguageRTL();

    QPixmap pixmap("./images/avatar.png");
    QIcon icon(pixmap);

    //TODO send to server full data

    ui->add_attach_button->setText(QString("Attach files"));
    ui->add_attach_button->setProperty("attached", {});
    ui->text_edit->clear();
}

void ChatClient::on_attach_files()
{
    // browse for image files and get a multiple selection
    auto const& file_names = QFileDialog::getOpenFileNames(this, tr("Select files..."), QDir::homePath(), tr("Images (*.png *.xpm *.jpg *.jpeg)"), nullptr, QFileDialog::Options(QFileDialog::ReadOnly));
    if (file_names.isEmpty())
    {
        ui->add_attach_button->setText(QString("Attach files"));
        ui->add_attach_button->setProperty("attached", {});
        return;
    }


    if (file_names.size() > 3)
    {
        QMessageBox::warning(this, tr("Warning"), tr("You can attach up to 3 files!"));
        return;
    }

    ui->add_attach_button->setText(QString("Attached %1 files").arg(file_names.size()));
    ui->add_attach_button->setProperty("attached", file_names);
}

void ChatClient::on_image_clicked(const QString& image_path)
{
   /* ui.imgLabel->setPixmap(QPixmap(image_path));
    ui.stackedWidget->setCurrentIndex(1);*/
}

//TODO redefine
void ChatClient::keyPressEvent(QKeyEvent* event_)
{
    /*if (event_->key() == Qt::Key_Escape)
    {
        ui.stackedWidget->setCurrentIndex(0);
        return;
    }
    QMainWindow::keyPressEvent(event);*/
}

void ChatClient::onReactionClick(const Likes& mes_user_likes_) 
{
    //TODO send to client data
    //TODO delete
    likeReceivedServer({ Like_enum::LIKE, "1", "Lisa"});

}


//-------------------------------SERVER-------------------------------
//-----Start Page
// 
//Client initiate connection to server
void ChatClient::attemptConnection()
{
    if (client->socketInfo()->state() == QAbstractSocket::UnconnectedState)
    {
        // We ask the user for the address of the server, we use 127.0.0.1 (aka localhost) as default
        const QString hostAddress = config_data.getConfig().getConfServer();
        const quint16 portAddress = config_data.getConfig().getConfPort();
        if (!hostAddress.isEmpty())
            client->connectToServer(QHostAddress(hostAddress), portAddress);
    }
    else
    {
        PLOGE << "No info about PORT";
        client->disconnectFromHost();
        //initConnection(); Снова связать рэдирид и делитпотом
    }
}

//--Start Page -> Login Page 
//If client connected succesfuly
void ChatClient::connectedToServer()
{
    PLOGI << "Client connected correctly";
    ui->login_nickname_edit->setText(config_data.getConfig().getConfNickname());
    ui->login_password_edit->setText(config_data.getConfig().getConfPassword());
    ui->login_password_edit->setEchoMode(QLineEdit::Password);
    ui->stackedWidget->setCurrentIndex(1);    
}

//-----LogIn Page
//If client put wrong data or already exist on server, use this function
void ChatClient::loginFailed(const QString& reason)
{
    QMessageBox::critical(this, tr("Error"), reason);

    ui->login_nickname_edit->clear();
    ui->login_password_edit->clear();
}

//--LogIn Page -> Profile Page 
//If client made a loging correctly, use this functuon
void ChatClient::loggedIn(const UserItem& user_info_)
{
    config_data.saveConfig(ui->login_nickname_edit->text(), ui->login_password_edit->text());



    ui->profile_edit_save_button->setText("Edit");
    ui->profile_start_chating_button->setEnabled(true);

    //Now they empty, becouse from server nothing was comming
    ui->profile_nickname_edit->setText(client->getUserNickname());
    ui->profile_password_edit->setText(client->getUserPassword());

    //TODO save config, when in client will bw all info

    ui->profile_nickname_edit->setEnabled(false);
    
    QPixmap pixmap(client->getUserAvatarPath());
    QPixmap scaledPixmap = pixmap.scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Set the pixmap to the QLabel
    ui->profile_image_lable->setPixmap(scaledPixmap);
    ui->profile_raiting_text->setText(QString::number(client->getUserRating()));
    ui->stackedWidget->setCurrentIndex(2);
}

//-----Profile Page
//If client create a user correctly, use this function
void ChatClient::userCreated(const UserItem& user_) {
    config_data.getConfig().saveConfig(ui->login_nickname_edit->text(), ui->login_password_edit->text());

    //TODO add about iqon
    ui->profile_edit_save_button->setText("Edit");
    ui->profile_nickname_edit->setEnabled(false);

    PLOGI << "User" << user_.getUserNickname() << "created correctly";
}

//If client make a mistake, when create a user, use this function
void ChatClient::createUserFailed(const QString& reason) {
    QMessageBox::critical(this, tr("Error"), reason);

    ui->profile_nickname_edit->clear();
    ui->profile_nickname_edit->clear();

    PLOGW << "Creating user finish with error" << reason;
}

//If client made changes with his profile, use this function
void ChatClient::userEdited(const UserItem& user_) {
    config_data.getConfig().saveConfig(ui->profile_nickname_edit->text(), ui->profile_password_edit->text());

    //TODO add about iqon
    ui->profile_edit_save_button->setText("Edit");
    ui->profile_nickname_edit->setEnabled(false);

    PLOGI << "User" << user_.getUserNickname() << "changed correctly";
}

//-----ChatList Page
// 
//--ChatList Page -> Chat Create Page 
//If topics comes, use this function
void ChatClient::topicsComes(const QStringList& topics_) 
{
    for (const QString& topic : topics_) {
        ui->add_room_combo_box->addItem(topic);
    }
    ui->stackedWidget->setCurrentIndex(5);
}

//-----Chat Create Page
// 
//--Chat Create Page -> ChatList Page 
//If chat created correctly, use this function
void ChatClient::roomCreated(const ChatItem& chat_) 
{
    //TODO delete
    Q_EMIT new_chat(
        QVariant::fromValue<chatItemPtr>
        (
            chatItemPtr{ new ChatItem(
                  chat_.getChatId()
                , chat_.getChatRoomName()
                , chat_.getChatRoomDescription()
                , chat_.getChatRoomTopicName()
                , chat_.getChatRoomIsPrivate()
                , chat_.getChatRoomPassword()) }
        )
    );
}

void ChatClient::connectedToRoom(const QList<MessageItem>& list_of_mess) 
{
 //TODO ask client for sending n-messages   
}

//If new mess recieved (download list of messages)
void ChatClient::messageListReceived(const QList<MessageItem>& list_of_mess)
{
    QVariantList var_list;
    for (const MessageItem& mes_item : list_of_mess) {
        var_list.append(
            QVariant::fromValue<messageItemPtr>
            (
                messageItemPtr{ new MessageItem(
                    mes_item.getMesId()
                    , mes_item.getMesNickname()
                    , mes_item.getMesText()
                    , mes_item.isRtl()
                    , mes_item.getMesListLikes()
                    , mes_item.getMesMediaId()
                    , mes_item.getMesAvatar()
                ) }
            )
        );
    }
    
    Q_EMIT download_messages(var_list);
}

//If new mess recieved
void ChatClient::messageReceived(const MessageItem& msg_)
{

    Q_EMIT new_message(
        QVariant::fromValue<messageItemPtr>
        (
            messageItemPtr{ new MessageItem(
                msg_.getMesId()
                , msg_.getMesNickname()
                , msg_.getMesText()
                , msg_.isRtl() //TODO how to understand] for new messages. Seems it should be inside JSON on server
                , msg_.getMesListLikes()
                , msg_.getMesMediaId() //TODO change. Here wil just ID of the picture. Need to convert ATTACH
                , msg_.getMesAvatar()) } 
        )
    );
}

////If new likes/dislikes recieved
void ChatClient::likeReceivedServer(const Likes& like_)
{

    Q_EMIT recivedLike(
        QVariant::fromValue<likeItemPtr>
        (
            likeItemPtr{ new Likes(
                like_.getLikeReaction()
                , like_.getLikeChatId()
                , like_.getLikeUserName()) }
    ));
}


//-----Others
//If the client was disconnected, use this function
void ChatClient::disconnectedFromServer()
{
    QMessageBox::warning(this, tr("Disconnected"), tr("The host terminated the connection"));
    ui->stackedWidget->setCurrentIndex(0);
}



//TODO if Server don't work - i need reaction
//TODO if Server closed - i need reaction
//