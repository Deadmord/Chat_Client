#pragma once

#include <QDateTime>
#include <QIcon>
#include <plog/Log.h> 
#include <QFile>
#include <QResource>


class UserItem : public QObject, public QEnableSharedFromThis<UserItem>
{
	Q_OBJECT;
public:
	struct userLoginStruct {
		QString user_nickname;
		QString user_password;
	};

	UserItem() : QObject(Q_NULLPTR) {};
	UserItem(
		QString		user_nickname_,
		int			user_rating_ = 0,
		QByteArray	user_data_ = {},
		QString user_password_ = "") :
		QObject(nullptr),
		user_nickname(std::move(user_nickname_)),
		user_password(std::move(user_password_)),
		user_rating(user_rating_),
		user_avatar(getUserAvatarIcon(user_nickname_, user_data_))
	{};

	~UserItem() override = default;
	UserItem(const UserItem&) = delete;
	UserItem(UserItem&&) = delete;
	const UserItem& operator =(const UserItem&) = delete;
	UserItem& operator = (UserItem&&) = delete;

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

	QIcon getUserAvatarIcon(const QString& user_nickname_, const QByteArray& user_avatar_data_)
	{
		QIcon default_icon;
		if (!user_nickname_.isNull()) {
			QString path = ":/ChatClient/images/" + user_nickname_ + ".png";
			QResource res_path(path);
			QFile file(path);
			if (res_path.isValid()) {
				default_icon = QIcon(path);
				PLOGI << "Avatar "<< user_nickname <<" exist in.qrc";
			}
			else {
				if (file.open(QIODevice::WriteOnly))
				{
					if (!user_avatar_data_.isEmpty()) {
						file.write(user_avatar_data_);
						file.close();
						default_icon = QIcon(path);
						PLOGI << "Image Create successfully.";
					}
					else {
						default_icon = QIcon(":/ChatClient/images/avatar.png");
						PLOGW << "No data in QByte array. used standart avatar";
					}
				}
				else
				{
					default_icon = QIcon(":/ChatClient/images/avatar.png");
					PLOGW << "Failed to save image. used standart avatar";
				}
			}
		}
		else {
			default_icon = QIcon(":/ChatClient/images/avatar.png");
			PLOGW << "No data comes from server. Used standart avatar";
		}

		return default_icon;
	}

private:
	QString			user_nickname;
	QString			user_password;

	QIcon			user_avatar{};
	int				user_rating{ 0 };
};

using UserItemPtr = QSharedPointer<UserItem>;
Q_DECLARE_METATYPE(UserItemPtr);
