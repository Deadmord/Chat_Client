#ifndef DTOMESSAGE_H
#define DTOMESSAGE_H
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QString>
#include <QSharedPointer>
#include "LikeItem.h"
#include "MessageItem.h"

class DTOMessage {

private:
	QString message_id;
	QString message_nickname;
	QString message_text;
	const bool is_rtl = false;
	listLikes message_list_likes;
	QString message_image_id;

public:
	DTOMessage();
	DTOMessage(const QString& message_id_, const QString& message_nickname_, const QString& message_text_, const bool& is_rtl_, const listLikes& message_list_likes_, const QString& message_image_id_);

	QString getMessageId() const;
	void setMessageId(const QString& message_id_);
	QString getMessageNickname() const;
	void setMessageNickname(const QString& message_nickname_);
	QString getMessageText() const;
	void setMessageText(const QString& message_text_);
	bool getRTL() const;
	listLikes getMessageListLike() const;
	void setMessageListLikes(const listLikes& message_list_likes_);
	QString getMessageImageId() const;
	void setMessageImageId(const QString& message_image_id_);

	static QSharedPointer<MessageItem> createMessageItemFromDTO(const DTOMessage& dto_message);
	static QSharedPointer<DTOMessage> createDTOMessageFromMessageItem(const MessageItem& message_item_);
	static bool toDTOMessageFromJson(DTOMessage& user_masg_dto_, const QJsonObject& user_msg_);
	static bool toJsonFromDTOMessage(QJsonObject& user_msg_, const DTOMessage& user_masg_dto_);
};

#endif // !DTOMESSAGE_H
