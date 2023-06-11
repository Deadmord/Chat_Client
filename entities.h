#ifndef ENTITIES_H
#define ENTITIES_H
#include <QTime>
#include <QPushButton>

struct MessageStruct {
    QString mes_nickname;
    QString mes_text;
    QString mes_id_chat;
    QDateTime mes_time;
    bool mes_is_deleted = true;
};

struct LogInWStruct {
    QString log_in_nickname;
    QString log_in_password;
    bool log_in_is_exist = false;
};

struct ButtonData
{
    QString text;
    // Add any other necessary properties for the button
};


//struct ButtonData {
//    QPushButton* button1;
//    QPushButton* button2;
//};

#endif // ENTITIES_H
