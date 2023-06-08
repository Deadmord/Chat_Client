#ifndef ENTITIES_H
#define ENTITIES_H
#include <QTime>
#include <QPushButton>

struct MessageStruct {
    QString mes_nickname;
    QString mes_text;
    QString mes_id_chat;
    QDateTime mes_time;
    bool mes_is_deleted;
};

//struct ButtonData {
//    QPushButton* button1;
//    QPushButton* button2;
//};

#endif // ENTITIES_H
