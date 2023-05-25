#include "client.h"

Client::Client(QString user_name_, QString user_password_, quint16 room_number_)
    :user_name(user_name_), user_password(user_password_), room_number(room_number_)
{
    last_message_time = QDateTime();
}

Client::~Client()
{

}

//------------getters-----------
const QString& Client::getUserName() {return user_name;}

const QString &Client::getUserPassword() {return user_password;}

quint16 Client::getRoomNum() {return room_number;}

const QDateTime &Client::getLastMessageTime() {return last_message_time;}

//------------setters-----------
void Client::setUserName(QString user_name_)
{
    this->user_name = user_name_;
}

void Client::setUserPassword(QString user_password_)
{
    this->user_password = user_password_;
}

void Client::setRoomNum(quint16 room_number_)
{
    this->room_number = room_number_;
}

void Client::setLastMessageTime()
{
    this->last_message_time = QDateTime::currentDateTime();
}

