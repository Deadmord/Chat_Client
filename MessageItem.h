#pragma once

#include <QDateTime>
#include <QIcon>
#include <ranges>

#include "Styles.h"


inline static QIcon getAvatarIcon(const int avatar_id)
{
	// TODO: implement getting images from custom storage
	// here, just for test, we use default icon

	static QIcon default_icon = QIcon(":/ChatClient/images/avatar.png");
	return default_icon;
}

//Q_DECLARE_METATYPE(QList<QPixmap>)

class MessageItem : public QObject, public QEnableSharedFromThis<MessageItem>
{
	Q_OBJECT;
public:
	QString	EXAMPLENAME = "Anton"; //TODO change for name from client/config

	struct messageToSend {
		QString message_text;
		QList<QPixmap> message_attach;
		quint16 message_room;
	};

	explicit MessageItem() : QObject(Q_NULLPTR) {};
	explicit MessageItem(
		QString message_nickname_,
		QString message_text_,
		const bool is_rtl_,
		QStringList message_file_list_,
		const QIcon& avatar_ = {}) : QObject(nullptr),
		message_nickname(std::move(message_nickname_)),
		message_text(std::move(message_text_)),
		message_is_rtl(is_rtl_),
		message_file_list(std::move(message_file_list)),
		message_avatar(avatar_.isNull() ? getAvatarIcon(0) : avatar_)
	{
		message_is_own = (message_nickname == EXAMPLENAME);
		message_image_list.clear();
		for (const auto& path : message_file_list)
		{
			message_image_list.push_back(QPixmap(path).scaled(IMAGE_PREVIEW_SIZE_MAX, Qt::KeepAspectRatio));
		}
		message_image_count = static_cast<int>(message_image_list.size());
		message_likes = 0;
	};

	~MessageItem() override = default;
	MessageItem(const MessageItem&) = delete;
	MessageItem(MessageItem&&) = delete;
	const MessageItem& operator =(const MessageItem&) = delete;
	MessageItem& operator = (MessageItem&&) = delete;

	auto shared() { return sharedFromThis(); }

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

	auto changeMesLikes(const int val) { message_likes += val; Q_EMIT likes_changed(); }

	[[nodiscard]] auto isHovered() const { return message_is_hovered; }
	auto setIsHovered(const bool val) { message_is_hovered = val; Q_EMIT hovered_changed(); }

	[[nodiscard]] auto const& getMesFilelist() const { return message_file_list; }
	auto setMesFilelist(const QStringList& val)
	{
		message_image_list.clear();
		
		std::ranges::transform(val, message_image_list.begin(), [](const auto& path) { return QPixmap(path).scaled(IMAGE_PREVIEW_SIZE_MAX, Qt::IgnoreAspectRatio); });
		message_file_list = val;
		message_image_count = static_cast<int>(message_image_list.size());
	}

	[[nodiscard]] auto const& getMesImagelist() const { return message_image_list; }

	[[nodiscard]] auto const& getMesAvatar() const { return message_avatar; }
	auto setMesAvatar(const QIcon& val) { message_avatar = val; Q_EMIT  avatar_changed(); }

	[[nodiscard]] auto getMesToSend() const
	{
		return messageToSend{ message_text, message_image_list , 1};
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


	[[nodiscard]] auto const& getImageCount() const { return message_image_count; }

	[[nodiscard]] auto isRtl() const { return message_is_rtl; }


	//QVariant toVariant() const
	//{
	//	QVariant variant;
	//	variant.setValue(*this);
	//	return variant;
	//}

	//MessageItem fromVariant(const QVariant& variant)
	//{
	//	return variant.value<MessageItem>();
	//}

Q_SIGNALS:
	void avatar_changed();
	void likes_changed();
	void hovered_changed();

private:
	bool			message_is_hovered{ false };

	QString			message_nickname;
	QString			message_text;
	QIcon			message_avatar{};
	bool			message_is_own{ false };
	QDateTime		message_time_stamp{ QDateTime::currentDateTime() };

	QList<QPixmap>	message_image_list {};
	int				message_image_count{ 0 };
	QStringList		message_file_list;

	int				message_likes{ 0 };

	QSize			message_current_text_box_size{};
	QRect			message_current_text_box_rect{};
	QRect			message_content_box_rect{};
	QList<QRect>	message_image_box_rects{};
	QRect			message_like_butt_rect{};
	QRect			message_dislike_butt_rect{};

	bool			message_is_rtl{ false };
	int				message_current_row{ -1 };
};

using messageItemPtr = QSharedPointer<MessageItem>;
using messagesList = QList<messageItemPtr>;
Q_DECLARE_METATYPE(messageItemPtr);
