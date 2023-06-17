#pragma once

#include <QDateTime>
#include <QIcon>

inline static QIcon getUserAvatarIcon(const int avatar_id)
{
	// TODO: implement getting images from custom storage
	// here, just for test, we use default icon

	static QIcon default_icon = QIcon(":/ChatClient/images/avatar.png");
	return default_icon;
}

class UserItem : public QObject, public QEnableSharedFromThis<UserItem>
{
	Q_OBJECT;
public:
	struct userLoginStruct {
		QString user_nickname;
		QString user_password;
	};

	static UserItem* instance()
	{
		static UserItem instance;
		return &instance;
	}

	auto shared() { return sharedFromThis(); }

	[[nodiscard]] auto getUserNickname() const { return user_nickname; }

	[[nodiscard]] auto getUserPassword() const { return user_password; }
	auto setUserPassword(const QString val) { user_password = val; }

	[[nodiscard]] auto const& getUserAvatar() const { return user_avatar; }
	auto setUserAvatar(const QIcon& val) { user_avatar = val; Q_EMIT  avatar_changed(); }

	[[nodiscard]] auto getUserRating() const { return user_rating; }
	auto setUserRating(const int val) { user_rating = val; Q_EMIT  rating_changed(); }

	[[nodiscard]] auto getUserLoginStruct() const
	{
		return userLoginStruct{ user_nickname, user_password };
	}


Q_SIGNALS:
	void avatar_changed();
	void rating_changed();

private:
	explicit UserItem() : QObject(Q_NULLPTR) {};
	explicit UserItem(
		QString user_nickname_,
		QString user_password_,
		int		user_rating_ = 0,
		const QIcon&	user_avatar_ = {}) : 
			QObject(nullptr),
			user_nickname(std::move(user_nickname_)),
			user_password(std::move(user_password_)),
			user_rating(user_rating_),
			user_avatar(user_avatar_.isNull() ? getUserAvatarIcon(0) : user_avatar_)
	{};

	~UserItem() override = default;
	UserItem(const UserItem&) = delete;
	UserItem(UserItem&&) = delete;
	const UserItem& operator =(const UserItem&) = delete;
	UserItem& operator = (UserItem&&) = delete;

private:
	QString			user_nickname;
	QString			user_password;

	QIcon			user_avatar{};
	int				user_rating{ 0 };
};

using UserItemPtr = QSharedPointer<UserItem>;
Q_DECLARE_METATYPE(UserItemPtr);
