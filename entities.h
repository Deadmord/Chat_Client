#ifndef ENTITIES_H
#define ENTITIES_H
#include <QTime>

struct Message {
    QString nickname;
    QString text;
    QTime time;
    QString id;
    bool deleted;

//    #include <QUuid>
//    void generateId() {
//        id = QUuid::createUuid().toString();
//    }
};

#endif // ENTITIES_H
