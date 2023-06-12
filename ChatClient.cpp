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

ChatClient::ChatClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatClientClass())
    , client(Client::instance())
{
    ui->setupUi(this);

    ui->text_edit->setPlaceholderText("Enter message text here");

    ui->stackedWidget->setCurrentIndex(2);


    //connect(ui.plainTextEdit, &QPlainTextEdit::textChanged, this, &ChatClient::on_message_text_changed);
    connect(ui->add_attach_button, &QPushButton::clicked, this, &ChatClient::on_attach_files);

    connect(this, &ChatClient::new_message, ui->chat_listView, &MessageWView::onMessageAdded);

    connect(ui->login_sign_in_button, &QPushButton::clicked, this, &ChatClient::on_sign_in_button_clicked);
    connect(ui->send_button, &QPushButton::clicked, this, &ChatClient::on_sendButton_clicked);

}

ChatClient::~ChatClient()
{
    // delete the elements created from the .ui file
    delete ui;
}

void ChatClient::on_log_in_button_clicked() 
{
    LogInStruct logIn_data_for_send = { ui->login_nickname_edit->text(), ui->login_password_edit->text()};    
    //TODO sending logIn_data_for_send to server for checking
}

void ChatClient::on_sign_in_button_clicked()
{
    ui->profile_edit_save_button->setText("Save");
    ui->profile_start_chating_button->setEnabled(false);
    QPixmap pixmap("./images/avatar.png");

    // Set the pixmap to the QLabel
    ui->profile_image_lable->setPixmap(pixmap);
    ui->stackedWidget->setCurrentIndex(3);
}

void ChatClient::startClient()
{
    loadConfig(CONFIG_FILE_PATH);                       //loading configuration settings
    ui->login_nickname_edit->setText(client.getUserNickname());
    ui->login_password_edit->setText(client.getUserPassword());

    //saveConfig(CONFIG_FILE_PATH);

    //ui->serverIPLineEdit->setText(server_address);                  //вынести в функцию uiInit()
    //ui->serverPortLineEdit->setText(QString::number(server_port));
    //ui->nickNameLineEdit->setText(client.getUserName());
    //ui->passwordLineEdit->setText(client.getUserPassword());
    //ui->roomLineEdit->setText(QString::number(client.getRoomNum()));

    initConnection();
}

void ChatClient::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if (in.status() == QDataStream::Ok)
    {
        //        QString str;
        //        in >> str;
        //        ui->textBrowser->append(str);
        for (;;)
        {
            //ToDo: looks like it sould be in separate thread, couse a waiting
            if (nextBlockSize == 0)
            {
                if (socket->bytesAvailable() < 2)
                {
                    break;
                }
                in >> nextBlockSize;
            }
            if (socket->bytesAvailable() < nextBlockSize)
            {
                break;
            }
            //strange conditionб warning
            Message msg;
            in >> msg.id >> msg.time >> msg.nickname >> msg.deleted >> msg.text;

            nextBlockSize = 0;
            if (!msg.deleted)                //TODO Create printmessage function
            {
                ui->textBrowser->append(msg.id + " " + msg.time.toString() + " " + msg.nickname + " :\t" + msg.text);
            }
        }
    }
    else
    {
        ui->textBrowser->append("Read error");
    }
}

void ChatClient::slotDisconnect()
{
    socket->deleteLater();
    ui->textBrowser->append("Server disconnected!");
}

void ChatClient::sendToServer(Message msg)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);

    out << quint16(0) << msg.id << msg.time << msg.nickname << msg.deleted << msg.text; // ToDo: define operators << and >> for "Messege"

    out.device()->seek(0);          //jamp to start block
    out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);
    ui->lineEdit->clear();
}
void ChatClient::initConnection()
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatClient::slotDisconnect);
}
Message ChatClient::createMessage(QString nickame, QString text)
{
    return Message{ nickame, text, QDateTime::currentDateTime(), QUuid::createUuid().toString(), false };
}

void ChatClient::loadConfig(QString _path)
{
    QFile config_file;
    QJsonDocument config_file_doc;
    QJsonObject config_json;
    QJsonParseError jsonError;

    config_file.setFileName(_path);

    if (config_file.open(QIODevice::ReadOnly | QFile::Text))
    {
        config_file_doc = QJsonDocument::fromJson(QByteArray(config_file.readAll()), &jsonError);
        config_file.close();

        if (jsonError.error == QJsonParseError::NoError)
        {
            configFromJson(config_file_doc);
        }
        else
        {
            qWarning() << "Error config file read: " << jsonError.error;
        }
    }
    else
    {
        qWarning("Error configuration file cannot be opened.");
    }
}

void ChatClient::saveConfig(QString _path)
{
    QFile config_file;
    QJsonDocument config_file_doc;
    QJsonObject config_json;
    QJsonParseError jsonError;

    config_file.setFileName(_path);
    if (config_file.open(QIODevice::WriteOnly | QFile::Text))
    {
        config_json = configToJson();
        config_file.write(QJsonDocument(config_json).toJson());
    }
    else
    {
        qDebug() << "Error configuration file cannot be opened.";
    }
    config_file.close();
}

void ChatClient::configFromJson(const QJsonDocument& config_file_doc_)
{
    QJsonObject config_json = config_file_doc_.object();

    if (const QJsonValue v = config_json["ServerAddress"]; v.isString())
        server_address = v.toString();
    else
        qWarning() << "Error ServerAddress reading";

    if (const QJsonValue v = config_json["ServerPort"]; v.isDouble())
        server_port = v.toInt();
    else
        qWarning() << "Error ServerPort reading";

    if (const QJsonValue v = config_json["FloodLimit"]; v.isDouble())
        flood_limit = v.toInt();
    else
        qWarning() << "Error FloodLimit reading";

    if (const QJsonValue v = config_file_doc_["User"]["Nickname"]; v.isString())
        client.setUserNickname(v.toString());
    else
        qWarning() << "Error LastRoomNumber reading";

    if (const QJsonValue v = config_file_doc_["User"]["Password"]; v.isString())
        client.setUserPassword(v.toString());
    else
        qWarning() << "Error LastRoomNumber reading";

    if (const QJsonValue v = config_file_doc_["User"]["LastRoomNumber"]; v.isDouble())
        client.setRoomNum(v.toInt());
    else
        qWarning() << "Error LastRoomNumber reading";

    if (const QJsonValue v = config_file_doc_["MessagesHistorySettings"]["Path"]; v.isString())
        msg_history_path = v.toString();
    else
        qWarning() << "Error MessagesHistorySettings path reading";
}

QJsonObject ChatClient::configToJson()
{
    QJsonObject json;
    QJsonObject user;
    QJsonObject history;

    json["ServerAddress"] = server_address;
    json["ServerPort"] = server_port;
    json["FloodLimit"] = flood_limit;
    user["Nickname"] = client.getUserNickname();
    user["Password"] = client.getUserPassword();
    user["LastRoomNumber"] = client.getRoomNum();
    json["User"] = user;
    history["Path"] = msg_history_path;
    json["MessagesHistorySettings"] = history;

    return json;
}

void ChatClient::keepCurrentConfig()
{
    //client.setUserName(ui->nickNameLineEdit->text());
    server_address = (ui->serverIPLineEdit->text());            //вынести в отдельную функцию или слот
    server_port = (ui->serverPortLineEdit->text().toUInt());    //для чтения и записи config использовать структуру, где формировать обьект конфига
    client->setUserName(ui->nickNameLineEdit->text());
    client->setUserPassword(ui->passwordLineEdit->text());
    client->setRoomNum(ui->roomLineEdit->text().toUInt());

    saveConfig(CONFIG_FILE_PATH);
}


//void ChatClient::sendToServer(Message msg)
//{
//    Data.clear();
//    QDataStream out(&Data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_6_2);
//
//    out << quint16(0) << msg.id << msg.time << msg.nickname << msg.deleted << msg.text; // ToDo: define operators << and >> for "Messege"
//
//    out.device()->seek(0);          //jamp to start block
//    out << quint16(Data.size() - sizeof(quint16));
//    socket->write(Data);
//    ui->messageEdit->clear();
//}

//Message ChatClient::createMessage(QString nickame, QString text)
//{
//    return Message{ nickame, text, QDateTime::currentDateTime(), QUuid::createUuid().toString(), false };
//}

//-----Old finish------------------------

//-----New start-------------------------


void ChatClient::attemptConnection()
{
    if (client->socketInfo()->state() == QAbstractSocket::UnconnectedState)
    {
        // We ask the user for the address of the server, we use 127.0.0.1 (aka localhost) as default
        const QString hostAddress = QInputDialog::getText(
            this
            , tr("Chose Server")
            , tr("Server Address")
            , QLineEdit::Normal
            , server_address
        );
        if (hostAddress.isEmpty())
            return; // the user pressed cancel or typed nothing
        // disable the connect button to prevent the user clicking it again
        // tell the client to connect to the host using the port 5555
        client->connectToServer(QHostAddress(hostAddress), server_port);
    }
    else
    {
        client->disconnectFromHost();
        //initConnection(); Снова связать рэдирид и делитпотом
    }
}

void ChatClient::connectedToServer()
{
    ui->connectButton->setText("Disconnect");
    // once we connected to the server we ask the user for what username they would like to use
    const QString newUsername = QInputDialog::getText(this, tr("Chose Username"), tr("Username"), QLineEdit::Normal, client->getUserName());
    if (newUsername.isEmpty()) {
        // if the user clicked cancel or typed nothing, we just disconnect from the server
        return client->disconnectFromHost();
    }
    // try to login with the given username
    attemptLogin(newUsername);
}

void ChatClient::attemptLogin(const QString& userName)
{
    // use the client to attempt a log in with the given username
    client->login(userName);
}

void ChatClient::loggedIn()
{
    //client.setRoomNum(ui->roomLineEdit->text().toUInt());
}

void ChatClient::loginFailed(const QString& reason)
{
    // the server rejected the login attempt
    // display the reason for the rejection in a message box
    QMessageBox::critical(this, tr("Error"), reason);
    // allow the user to retry, execute the same slot as when just connected
    connectedToServer();
}

void ChatClient::messageReceived(const QString& sender, const QString& text)
{
    // store the index of the new row to append to the model containing the messages
    int newRow = chat_model->rowCount();
    // we display a line containing the username only if it's different from the last username we displayed
    if (last_user_name != sender) {
        // store the last displayed username
        last_user_name = sender;
        // create a bold default font
        QFont boldFont;
        boldFont.setBold(true);
        // insert 2 row, one for the message and one for the username
        chat_model->insertRows(newRow, 2);
        // store the username in the model
        chat_model->setData(chat_model->index(newRow, 0), sender + QLatin1Char(':'));
        // set the alignment for the username
        chat_model->setData(chat_model->index(newRow, 0), int(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextAlignmentRole);
        // set the for the username
        chat_model->setData(chat_model->index(newRow, 0), boldFont, Qt::FontRole);
        ++newRow;
    }
    else {
        // insert a row for the message
        chat_model->insertRow(newRow);
    }
    // store the message in the model
    chat_model->setData(chat_model->index(newRow, 0), text);
    // set the alignment for the message
    chat_model->setData(chat_model->index(newRow, 0), int(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextAlignmentRole);
    // scroll the view to display the new message
    ui->chatView->scrollToBottom();
}

void ChatClient::sendMessage()
{
    if (client->socketInfo()->state() == QAbstractSocket::ConnectedState)
    {
        // we use the client to send the message that the user typed
        client->sendMessage(ui->messageEdit->text());
        // now we add the message to the list
        // store the index of the new row to append to the model containing the messages
        const int newRow = chat_model->rowCount();
        // insert a row for the message
        chat_model->insertRow(newRow);
        // store the message in the model
        chat_model->setData(chat_model->index(newRow, 0), ui->messageEdit->text());
        // set the alignment for the message
        chat_model->setData(chat_model->index(newRow, 0), int(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
        // clear the content of the message editor
        ui->messageEdit->clear();
        // scroll the view to display the new message
        ui->chatView->scrollToBottom();
        // reset the last printed username
        last_user_name.clear();
    }
    else
    {
        const int newRow = chat_model->rowCount();
        chat_model->insertRow(newRow);
        chat_model->setData(chat_model->index(newRow, 0), "You are disconnected. Try reconnect, please...");
        chat_model->setData(chat_model->index(newRow, 0), int(Qt::AlignCenter | Qt::AlignVCenter), Qt::TextAlignmentRole);
        ui->chatView->scrollToBottom();
        last_user_name.clear();
    }
}

void ChatClient::disconnectedFromServer()
{
    ui->connectButton->setText("Connect");
    // if the client loses connection to the server
    // comunicate the event to the user via a message box
    QMessageBox::warning(this, tr("Disconnected"), tr("The host terminated the connection"));
    // disable the ui to send and display messages
    ui->sendButton->setEnabled(false);
    ui->messageEdit->setEnabled(false);
    ui->chatView->setEnabled(false);
    // enable the button to connect to the server again
    ui->connectButton->setEnabled(true);
    // reset the last printed username
    last_user_name.clear();
}

void ChatClient::userJoined(const QString& username)
{
    // store the index of the new row to append to the model containing the messages
    const int newRow = chat_model->rowCount();
    // insert a row
    chat_model->insertRow(newRow);
    // store in the model the message to comunicate a user joined
    chat_model->setData(chat_model->index(newRow, 0), tr("%1 Joined the Chat").arg(username));
    // set the alignment for the text
    chat_model->setData(chat_model->index(newRow, 0), Qt::AlignCenter, Qt::TextAlignmentRole);
    // set the color for the text
    chat_model->setData(chat_model->index(newRow, 0), QBrush(Qt::blue), Qt::ForegroundRole);
    // scroll the view to display the new message
    ui->chatView->scrollToBottom();
    // reset the last printed username
    last_user_name.clear();
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
    const auto nickname = client.getUserNickname();
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

    //Create message function
    //Send message function
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        //sendToServer(createMessage(client.getUserName(), ui->lineEdit->text()));
    }
    else
    {
        //ui->textBrowser->append("Not Connected to Server");
        qDebug() << socket->state();
    }
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

void ChatClient::userLeft(const QString& username)
{
    // store the index of the new row to append to the model containing the messages
    const int newRow = chat_model->rowCount();
    // insert a row
    chat_model->insertRow(newRow);
    // store in the model the message to comunicate a user left
    chat_model->setData(chat_model->index(newRow, 0), tr("%1 Left the Chat").arg(username));
    // set the alignment for the text
    chat_model->setData(chat_model->index(newRow, 0), Qt::AlignCenter, Qt::TextAlignmentRole);
    // set the color for the text
    chat_model->setData(chat_model->index(newRow, 0), QBrush(Qt::red), Qt::ForegroundRole);
    // scroll the view to display the new message
    ui->chatView->scrollToBottom();
    // reset the last printed username
    last_user_name.clear();
}

void ChatClient::errorSlot(QAbstractSocket::SocketError socketError)
{
    // show a message to the user that informs of what kind of error occurred
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
    case QAbstractSocket::ProxyConnectionClosedError:
        return; // handled by disconnectedFromServer
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::critical(this, tr("Error"), tr("The host refused the connection"));
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        QMessageBox::critical(this, tr("Error"), tr("The proxy refused the connection"));
        break;
    case QAbstractSocket::ProxyNotFoundError:
        QMessageBox::critical(this, tr("Error"), tr("Could not find the proxy"));
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::critical(this, tr("Error"), tr("Could not find the server"));
        break;
    case QAbstractSocket::SocketAccessError:
        QMessageBox::critical(this, tr("Error"), tr("You don't have permissions to execute this operation"));
        break;
    case QAbstractSocket::SocketResourceError:
        QMessageBox::critical(this, tr("Error"), tr("Too many connections opened"));
        break;
    case QAbstractSocket::SocketTimeoutError:
        QMessageBox::warning(this, tr("Error"), tr("Operation timed out"));
        return;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        QMessageBox::critical(this, tr("Error"), tr("Proxy timed out"));
        break;
    case QAbstractSocket::NetworkError:
        QMessageBox::critical(this, tr("Error"), tr("Unable to reach the network"));
        break;
    case QAbstractSocket::UnknownSocketError:
        QMessageBox::critical(this, tr("Error"), tr("An unknown error occured"));
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        QMessageBox::critical(this, tr("Error"), tr("Operation not supported"));
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        QMessageBox::critical(this, tr("Error"), tr("Your proxy requires authentication"));
        break;
    case QAbstractSocket::ProxyProtocolError:
        QMessageBox::critical(this, tr("Error"), tr("Proxy comunication failed"));
        break;
    case QAbstractSocket::TemporaryError:
    case QAbstractSocket::OperationError:
        QMessageBox::warning(this, tr("Error"), tr("Operation failed, please try again"));
        return;
    default:
        Q_UNREACHABLE();
    }
    // enable the button to connect to the server again
    ui->connectButton->setEnabled(true);
    // disable the ui to send and display messages
    ui->sendButton->setEnabled(false);
    ui->messageEdit->setEnabled(false);
    ui->chatView->setEnabled(false);
    // reset the last printed username
    last_user_name.clear();
}
