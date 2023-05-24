#include "ChatClient.h"

ChatClient::ChatClient(Client& client, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatClientClass())
    , client(client)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatClient::slotDisconnect);
}

ChatClient::~ChatClient()
{
    delete ui;
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
Message ChatClient::createMessage(QString nickame, QString text)
{
    return Message{ nickame, text, QDateTime::currentDateTime(), QUuid::createUuid().toString(), false };
}
//-------------window interface------------
void ChatClient::on_nickNameLineEdit_returnPressed()
{
    client.setUserName(ui->nickNameLineEdit->text());
}

void ChatClient::on_serverIPLineEdit_returnPressed()
{
    client.setHostName(ui->serverIPLineEdit->text());
}

void ChatClient::on_serverPortLineEdit_returnPressed()
{
    client.setPort(ui->serverPortLineEdit->text().toUInt());
}

void ChatClient::on_roomLineEdit_returnPressed()
{
    client.setRoomNum(ui->roomLineEdit->text().toUInt());
}

void ChatClient::on_connectButton_clicked()
{
    client.setUserName(ui->nickNameLineEdit->text());       //try to save curent settings
    client.setHostName(ui->serverIPLineEdit->text());
    client.setPort(ui->serverPortLineEdit->text().toUInt());
    client.setRoomNum(ui->roomLineEdit->text().toUInt());

    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        if (client.getUserName().size() &&
            client.getHostName().size() &&
            client.getPort() &&
            client.getRoomNum())
        {
            socket->connectToHost(client.getHostName(), client.getPort());
            //with async await if(socket->state() == QAbstractSocket::ConnectedState)
            ui->connectButton->setText("Disconnect");
            //and only after that change button title
        }
        else
        {
            ui->textBrowser->append("Fill your name and server address, please.");
        }
    }
    else
    {
        socket->disconnectFromHost();
        ui->connectButton->setText("Connect");
        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::readyRead, this, &ChatClient::slotReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &ChatClient::slotDisconnect);
    }
}

void ChatClient::on_roomButton_clicked()
{

}

void ChatClient::on_sendButton_clicked()
{
    //Create message function
    //Send message function
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        sendToServer(createMessage(client.getUserName(), ui->lineEdit->text()));
    }
    else
    {
        ui->textBrowser->append("Not Connected to Server");
        qDebug() << socket->state();
    }
}
void ChatClient::on_lineEdit_returnPressed()
{
    on_sendButton_clicked();
}

