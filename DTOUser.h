#ifndef DTOUSER_H
#define DTOUSER_H

#include <QString>
#include <QSharedPointer>
#include "UserItem.h"

class DTOUser {

private:
	QString nickname;
	qint32 rating;
	QString userpic;
	QString password;

public:
	DTOUser(const QString& nickname_, const qint32& rating_, const QString& userpic_, const QString& password_ = "");

	QString getNickname() const;
	qint32 getRating() const;
	QString getUserpic() const;
	QString getPassword() const;
	void setNickname(const QString& nickname_);
	void setRating(const qint32& rating_);
	void setUserpic(const QString& userpic_);
	void setPassword(const QString& password_);

	static QSharedPointer<UserItem> createUserItemFromDTOUser(const DTOUser& dto_user_);
	static QSharedPointer<DTOUser> createDTOUserFromUserItem(const UserItem& user_item_);
};

#endif // !DTOUSER_H
