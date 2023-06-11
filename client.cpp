#include "client.h"

Client::Client(QString user_nickname_, QString user_password_, quint16 user_current_chat_room_number_)
    :user_nickname(user_nickname_), user_password(user_password_), user_current_chat_room_number(user_current_chat_room_number_)
{
    user_picture_path = QPixmap(initiate_picture_path);
    last_message_time = QDateTime();
    PLOGI << "client " << this->getUserNickname() << " was created";
}

Client::~Client()
{

}

//------------getters-----------
const QString& Client::getUserNickname() {return user_nickname;}

const QString &Client::getUserPassword() {return user_password;}

quint16 Client::getRoomNum() {return user_current_chat_room_number;}

const QDateTime &Client::getLastMessageTime() {return last_message_time;}

//------------setters-----------
void Client::setUserNickname(QString user_name_)
{
    this->user_nickname = user_name_;
}

void Client::setUserPassword(QString user_password_)
{
    this->user_password = user_password_;
}

void Client::setRoomNum(quint16 room_number_)
{
    this->user_current_chat_room_number = room_number_;
}

void Client::setLastMessageTime()
{
    this->last_message_time = QDateTime::currentDateTime();
}

