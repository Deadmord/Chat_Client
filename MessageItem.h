#pragma once

#include <QDateTime>
#include <QIcon>
#include <ranges>

#include "Styles.h"
#include "LikeItem.h"


inline static QIcon getAvatarIcon(const int avatar_id)
{
	// TODO: implement getting images from custom storage
	// here, just for test, we use default icon

	static QIcon default_icon = QIcon(":/ChatClient/images/avatar.png");
	return default_icon;
}

Q_DECLARE_METATYPE(QList<QPixmap>)

class MessageItem : public QObject, public QEnableSharedFromThis<MessageItem>
{
	Q_OBJECT;
public:
	QString	EXAMPLENAME = "Anton"; //TODO change for name from client/config

	struct messageToSend {
		QString message_text;
		QString message_attach;
	};

	explicit MessageItem() : QObject(Q_NULLPTR) {};
	explicit MessageItem(
		QString			message_id_,
		QString			message_nickname_,
		QString			message_text_,
		const bool		is_rtl_,
		QString			message_media_id_ = "",
		listLikes		message_list_likes_ = {},
		const QIcon&	avatar_ = {}) : QObject(nullptr),
		message_id(std::move(message_id_)),
		message_nickname(std::move(message_nickname_)),
		message_text(std::move(message_text_)),
		message_is_rtl(is_rtl_),
		message_media_id(std::move(message_media_id_)),
		message_list_likes(std::move(message_list_likes_)),
		message_avatar(avatar_.isNull() ? getAvatarIcon(0) : avatar_)
	{
		message_is_own = (message_nickname == EXAMPLENAME);
		/*message_image_list.clear();
		for (const auto& path : message_file_list)
		{
			message_image_list.push_back(QPixmap(path).scaled(IMAGE_PREVIEW_SIZE_MAX, Qt::KeepAspectRatio));
		}
		message_image_count = static_cast<int>(message_image_list.size());*/
		!message_list_likes.isEmpty() ? message_likes = message_list_likes.size() : 0;
	};

	~MessageItem() override = default;
	MessageItem(const MessageItem&) = delete;
	MessageItem(MessageItem&&) = delete;
	const MessageItem& operator =(const MessageItem&) = delete;
	MessageItem& operator = (MessageItem&&) = delete;

	auto shared() { return sharedFromThis(); }

	[[nodiscard]] auto getMesId() const { return message_id; }

	[[nodiscard]] auto getMesNickname() const { return message_nickname; }
	auto setMesNickname(const QString val) { message_nickname = val; }

	[[nodiscard]] auto isOwn() const { return message_is_own; }
	auto setIsOwn(const bool val) { message_is_own = val; }

	[[nodiscard]] const auto& getMesText() const { return message_text; }
	auto setMesText(const QString& val) { message_text = val; }

	[[nodiscard]] auto const& getMesTimeStamp() const { return message_time_stamp; }
	auto setMesTimeStamp(const QDateTime& val) { message_time_stamp = val; }

	[[nodiscard]] auto getMesLikes() const { return message_likes; }
	auto setMesLikes(const int val) { message_likes = val; Q_EMIT likes_changed(); }
	//Question: Why we use here Q_emit?

	auto changeMesLikes(const int val) { message_likes += val; Q_EMIT likes_changed(); }

	[[nodiscard]] auto isHovered() const { return message_is_hovered; }
	auto setIsHovered(const bool val) { message_is_hovered = val; Q_EMIT hovered_changed(); }

	[[nodiscard]] auto getMesUserReaction() const { return message_user_reaction; }
	auto setMesUserReaction(const Like_enum val) { message_user_reaction = val; }

	[[nodiscard]] auto getMesMediaId() const { return message_media_id; }
	auto setMesMediaId(const QString val) { message_media_id = val; }

	/*[[nodiscard]] auto const& getMesFilelist() const { return message_file_list; }
	auto setMesFilelist(const QStringList& val)
	{
		message_image_list.clear();
		
		std::ranges::transform(val, message_image_list.begin(), [](const auto& path) { return QPixmap(path).scaled(IMAGE_PREVIEW_SIZE_MAX, Qt::IgnoreAspectRatio); });
		message_file_list = val;
		message_image_count = static_cast<int>(message_image_list.size());
	}

	[[nodiscard]] auto const& getMesImagelist() const { return message_image_list; }
	*/

	[[nodiscard]] auto const& getMesAvatar() const { return message_avatar; }
	auto setMesAvatar(const QIcon& val) { message_avatar = val; Q_EMIT  avatar_changed(); }

	[[nodiscard]] auto getMesToSend() const
	{
		return messageToSend{ message_text, message_media_id };
	}

	[[nodiscard]] auto const& getCurrentRow() const { return message_current_row; }
	auto setCurrentRow(const int val) { message_current_row = val; }

	[[nodiscard]] auto const& getContentBox() const { return message_content_box_rect; }
	auto setContentBox(const QRect val) { message_content_box_rect = val; }

	[[nodiscard]] auto const& getCurrentTextBoxSize() const { return message_current_text_box_size; }
	auto setCurrentTextBoxSize(const QSize val) { message_current_text_box_size = val; }

	[[nodiscard]] auto const& getCurrentTextBoxRect() const { return message_current_text_box_rect; }
	auto setCurrentTextBoxRect(const QRect val) { message_current_text_box_rect = val; }

	[[nodiscard]] auto& getImageBoxRects() { return message_image_box_rects; }

	[[nodiscard]] auto const& getLikeButtRect() const { return message_like_butt_rect; }
	auto setLikeButtRect(const QRect val) { message_like_butt_rect = val; }

	[[nodiscard]] auto const& getDislikeButtRect() const { return message_dislike_butt_rect; }
	auto setDislikeButtRect(const QRect val) { message_dislike_butt_rect = val; }

	auto addNewLike(const likeItemPtr val) { 
		message_list_likes.emplaceBack(val);
		message_likes = val->getLikeReaction() == Like_enum::LIKE ? message_likes++: message_likes--;
	}

	[[nodiscard]] auto isLiked() {
		for (const likeItemPtr& like : message_list_likes) {
			like->getLikeUserName() == EXAMPLENAME ? true : false;
		}
	}



	//[[nodiscard]] auto const& getImageCount() const { return message_image_count; }

	[[nodiscard]] auto isRtl() const { return message_is_rtl; }

Q_SIGNALS:
	void avatar_changed();
	void likes_changed();
	void hovered_changed();

private:
	bool			message_is_hovered{ false };

	QString			message_id;
	QString			message_nickname;
	QString			message_text;
	QIcon			message_avatar{};
	bool			message_is_own{ false };
	QDateTime		message_time_stamp{ QDateTime::currentDateTime() };

	QString			message_media_id{};
	//QList<QPixmap>	message_image_list {};
	//int				message_image_count{ 0 };
	//QStringList		message_file_list;
	bool			message_is_rtl{ false };

	listLikes		message_list_likes;
	Like_enum		message_user_reaction{ Like_enum::NO_REACTION };
	int				message_likes{ 0 };

	QSize			message_current_text_box_size{};
	QRect			message_current_text_box_rect{};
	QRect			message_content_box_rect{};
	QList<QRect>	message_image_box_rects{};
	QRect			message_like_butt_rect{};
	QRect			message_dislike_butt_rect{};
	int				message_current_row{ -1 };

};

using messageItemPtr = QSharedPointer<MessageItem>;
using messagesList = QList<messageItemPtr>;
Q_DECLARE_METATYPE(messageItemPtr);
