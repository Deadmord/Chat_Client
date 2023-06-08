#include "client.h"

Client::Client(QString user_name_, QString user_password_, quint16 room_curr_number_)
    :user_nickname(user_name_), user_password(user_password_), user_curr_room_number(room_curr_number_)
{
}

Client::~Client()
{

}

//------------getters-----------
const QString& Client::getUserNickname() {return user_nickname;}

const QString &Client::getUserPassword() {return user_password;}

quint16 Client::getRoomNum() {return user_curr_room_number;}


//------------setters-----------
void Client::setUserNickname(QString user_nickname_)
{
    this->user_nickname = user_nickname_;
}

void Client::setUserPassword(QString user_password_)
{
    this->user_password = user_password_;
}

void Client::setRoomNum(quint16 room_curr_number_)
{
    this->user_curr_room_number = room_curr_number_;
}

