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
    , config_data()
{
    ui->setupUi(this);
    //ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(3);

    //Connections with view
    connect(this, &ChatClient::new_message, ui->chat_listView, &MessageWView::onMessageAdded);
    connect(this, &ChatClient::new_chat, ui->chatList_listView, &ChatWView::onChatAdded);

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

    //ChatRoomW
    connect(ui->send_button, &QPushButton::clicked, this, &ChatClient::on_sendButton_clicked);
    connect(ui->add_attach_button, &QPushButton::clicked, this, &ChatClient::on_attach_files);

    //Connections with Client
    connect(client, &Client::connected, this, &ChatClient::connectedToServer);
    connect(client, &Client::loginError, this, &ChatClient::loginFailed);
    connect(client, &Client::loggedIn, this, &ChatClient::loggedIn);
    connect(client, &Client::messageReceived, this, &ChatClient::messageReceived);

    connect(client, &Client::disconnected, this, &ChatClient::disconnectedFromServer); //TODO make a buttton to disconnect

}

ChatClient::~ChatClient()
{
    delete ui;
}

void ChatClient::onStartAppClicked() {
    attemptConnection();
}

void ChatClient::on_start_chatting_clicked() {
    ui->text_edit->setPlaceholderText("Enter message text here");
    ui->stackedWidget->setCurrentIndex(3);
}

void ChatClient::onAddChatButtonClicked()
{
    Q_EMIT new_chat(
        QVariant::fromValue<chatItemPtr>
        (
            chatItemPtr{ new ChatItem(
                1
                , "name"
                , "description sfdsg thgrthrthtrherh ttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt ttttttttttttt ttttttttttttttt ttttttttttttt ttttttttttfffffffffffffftttt "
                , "alable"
                , true) }
        )
    );
};

void ChatClient::on_log_in_button_clicked() 
{
    if (!ui->login_nickname_edit->text().isEmpty() && !ui->login_password_edit->text().isEmpty()) {
        client->login(ui->login_nickname_edit->text(), ui->login_password_edit->text());

        //TODO delete. Just for time
        loggedIn();
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

void ChatClient::on_sendButton_clicked()
{
    if (ui->text_edit->toPlainText().isEmpty())
        return;
    if (!ui->send_button->isEnabled())
        return;

    static auto ID = 0ULL;
    ++ID;

    //const auto icon = ui.comboBox->itemIcon(ui.comboBox->currentIndex());
    //const auto name = ui.comboBox->currentText();
    const auto nickname = "Anton";
    const auto text = ui->text_edit->toPlainText();
    auto const is_RTL = IsCurrentInputLanguageRTL();

    QPixmap pixmap("./images/avatar.png");
    QIcon icon(pixmap);

    Q_EMIT new_message(
        QVariant::fromValue<messageItemPtr>
        (
            messageItemPtr{ new MessageItem(
                //ID 
                nickname
                , text
                , is_RTL
                , ui->add_attach_button->property("attached").toStringList()
                , icon) }
        )
    );

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

void ChatClient::keyPressEvent(QKeyEvent* event_)
{
    /*if (event_->key() == Qt::Key_Escape)
    {
        ui.stackedWidget->setCurrentIndex(0);
        return;
    }
    QMainWindow::keyPressEvent(event);*/
}


//----Connect Server
void ChatClient::attemptConnection()
{
    if (client->socketInfo()->state() == QAbstractSocket::UnconnectedState)
    {
        // We ask the user for the address of the server, we use 127.0.0.1 (aka localhost) as default
        const QString hostAddress = config_data.getConfServer();
        const quint16 portAddress = config_data.getConfPort();
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

//If client connected succesfuly, use this function
void ChatClient::connectedToServer()
{
    PLOGI << "Client connected correctly";
    ui->login_nickname_edit->setText(config_data.getConfNickname());
    ui->login_password_edit->setText(config_data.getConfPassword());
    ui->stackedWidget->setCurrentIndex(1);    
}

void ChatClient::loginFailed(const QString& reason)
{
    QMessageBox::critical(this, tr("Error"), reason);

    ui->login_nickname_edit->clear();
    ui->login_password_edit->clear();
}

//If client made a loging correctly, use this functuon
void ChatClient::loggedIn()
{


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

//If the client was disconnected, use this function
void ChatClient::disconnectedFromServer()
{    
    QMessageBox::warning(this, tr("Disconnected"), tr("The host terminated the connection"));
    ui->stackedWidget->setCurrentIndex(0);
}

//If new mess recieved
void ChatClient::messageReceived(const MessageItem& msg_)
{
    Q_EMIT new_message(
        QVariant::fromValue<messageItemPtr>
        (
            messageItemPtr{ new MessageItem(
                //ID 
                msg_.getMesNickname()
                , msg_.getMesText()
                , msg_.isRtl() //TODO how to understand for new messages. Seems it should be inside JSON on server
                , msg_.getMesFilelist() //TODO change. Here wil just ID of the picture. Need to convert ATTACH
                , msg_.getMesAvatar()) }  //TODO change. Here wil just ID of the picture. Need to convert AVATAR
        )
    );
}