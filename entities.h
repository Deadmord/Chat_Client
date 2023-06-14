#ifndef ENTITIES_H
#define ENTITIES_H
#include <QTime>

struct Message {
    QString nickname;
    QString text;
    QDateTime time;
    QString id;
    bool deleted;
};



#endif // ENTITIES_H
