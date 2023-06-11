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

struct LogInStruct {
    QString log_in_nickname;
    QString log_in_password;
};

#endif // ENTITIES_H
