#pragma once

#include <QObject>
#include <QDateTime>
#include <QIcon>
#include <ranges>


class ChatItem : public QObject, public QEnableSharedFromThis<ChatItem>
{
	Q_OBJECT;
public:
	ChatItem() : QObject(Q_NULLPTR) {};
	ChatItem(
		qint32		id_,
		QString		room_name_,
		QString		description_,
		QString		chat_room_topic_name_,
		bool		is_private_,
		QString		password_ = "",
		bool		is_deleted_ = false) :
		QObject(Q_NULLPTR),
		chat_room_id(id_),
		chat_room_name(std::move(room_name_)),
		chat_room_description(std::move(description_)),
		chat_room_topic_name(std::move(chat_room_topic_name_)),
		chat_room_is_private(is_private_),
		chat_room_password(std::move(password_)),
		chat_room_is_deleted(is_deleted_)
	{};

	~ChatItem() override = default;
	ChatItem(const ChatItem&) = delete;
	ChatItem(ChatItem&&) = delete;
	const ChatItem& operator =(const ChatItem&) = delete;
	ChatItem& operator = (ChatItem&&) = delete;

	auto shared() { return sharedFromThis(); }

	[[nodiscard]] auto getChatId() const { return chat_room_id; };

	[[nodiscard]] auto getChatRoomName() const  { return chat_room_name; };
	void setChatRoomName(const QString& val_) { chat_room_name = val_; };

	[[nodiscard]] auto getChatRoomDescription() const { return chat_room_description; };
	void setChatRoomDescription(const QString& val_) { chat_room_description = val_; };

	[[nodiscard]] auto getChatRoomTopicName() const { return chat_room_topic_name; };
	void setChatRoomTopicName(const QString& val_) { chat_room_topic_name = val_; };

	[[nodiscard]] auto getChatRoomIsPrivate() const { return chat_room_is_private; };
	void setChatRoomIsPrivate(bool val_) { chat_room_is_private = val_; };

	[[nodiscard]] auto getChatRoomPassword() const { return chat_room_password; };
	void setChatRoomPassword(const QString& val_) { chat_room_password = val_; };

	[[nodiscard]] auto getChatRoomIsDeleted() const { return chat_room_is_deleted; };
	void setChatRoomIsDeleted(bool val_) { chat_room_is_deleted = val_; };

	[[nodiscard]] auto isHovered() const { return chat_room_is_hovered; }
	auto setIsHovered(const bool val) { chat_room_is_hovered = val; Q_EMIT hovered_changed(); }

	[[nodiscard]] auto getChatRoomCurrRow() const { return this->chat_current_row; };
	void setChatRoomCurrRow(int val_) { chat_current_row = val_; };

	[[nodiscard]] auto const& getChatRoomCurrBox() const { return chat_current_box; }
	auto setChatRoomCurrBox(const QRect val) { chat_current_box = val; }


Q_SIGNALS:
	void hovered_changed();


private:
	qint32		chat_room_id;
	QString		chat_room_name;
	QString		chat_room_description;
	QString		chat_room_topic_name;
	//Topic		chat_room_topic;
	bool		chat_room_is_private;
	QString		chat_room_password;
	bool		chat_room_is_deleted;
	bool		chat_room_is_hovered {false};

	int			chat_current_row{ -1 };
	QRect		chat_current_box{};
};


using chatItemPtr = QSharedPointer<ChatItem>;
using chatList = QList<chatItemPtr>;
Q_DECLARE_METATYPE(chatItemPtr); 