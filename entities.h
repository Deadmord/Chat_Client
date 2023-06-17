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

struct Topic {
    qint32		topic_id;
    QString		topic_name;
};




#endif // ENTITIES_H
