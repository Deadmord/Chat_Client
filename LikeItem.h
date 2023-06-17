#pragma once

#include <QString>
#include <QObject>
#include <QDateTime>
#include <QIcon>
#include <ranges>

enum Like_enum {
    NO_REACTION,
    LIKE,
    DISLIKE
};

class Likes : public QObject, public QEnableSharedFromThis<Likes> 
{

    Q_OBJECT;
public:
    Likes(
        Like_enum reaction_,
        QString   id_chat_,
        QString   user_name_) 
        : reaction(reaction_),
        id_chat(id_chat_),
        user_name(user_name_)
    {};

    Likes(const Likes&) = delete;
    Likes(Likes&&) = delete;
    const Likes& operator =(const Likes&) = delete;
    Likes& operator = (Likes&&) = delete;

    auto shared() { return sharedFromThis(); }

    auto getReaction() const { return reaction; };
    auto getIdChat() const { return id_chat; };
    auto getuserNamet() const { return user_name; };

private:
    Like_enum   reaction;
    QString     id_chat;
    QString     user_name;
};

using likeItemPtr = QSharedPointer<Likes>;
using listLikes = QList<likeItemPtr>;
Q_DECLARE_METATYPE(likeItemPtr);
