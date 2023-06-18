#include "DTOUser.h"

DTOUser::DTOUser(const QString& nickname_, const qint32& rating_, const QString& userpic_, const QString& password_)
	: nickname(nickname_), rating(rating_), userpic(userpic_), password(password_) {}

QString DTOUser::getNickname() const { return nickname; }
qint32 DTOUser::getRating() const { return rating; }
QString DTOUser::getUserpic() const { return userpic; }
QString DTOUser::getPassword() const { return password; }
void DTOUser::setNickname(const QString& nickname_) { nickname = nickname_; }
void DTOUser::setRating(const qint32& rating_) { rating = rating_; }
void DTOUser::setUserpic(const QString& userpic_) { userpic = userpic_; }
void DTOUser::setPassword(const QString& password_) { password = password_; }

QSharedPointer<UserItem> DTOUser::createUserItemFromDTOUser(const DTOUser& dto_user_) {
	QSharedPointer<UserItem> shp_user_item = QSharedPointer<UserItem>::create(
		dto_user_.getNickname(), dto_user_.getRating(), QByteArray::fromBase64(dto_user_.getUserpic().toLatin1()), dto_user_.getPassword());
	return shp_user_item;
}

//QSharedPointer<DTOUser> DTOUser::createDTOUserFromUserItem(const UserItem& user_item_) {
//	QSharedPointer<DTOUser> shp_dto_user = QSharedPointer<DTOUser>::create(
//		user_item_.getUserNickname(), user_item_.getUserRating(), user_item_.getUserAvatar(), user_item_.getUserPassword());
//	return shp_dto_user;
//}
