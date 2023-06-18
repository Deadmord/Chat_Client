#include "DTOMessage.h"

DTOMessage::DTOMessage(const QString& message_id_, const QString& message_nickname_, const QString& message_text_, const bool& is_rtl_, const listLikes& message_list_likes_, const QString& message_image_id_)
	: message_id(message_id_), message_nickname(message_nickname_), message_text(message_text_), is_rtl(is_rtl_), message_list_likes(message_list_likes_), message_image_id(message_image_id_) {};

QString DTOMessage::getMessageId() const { return message_id; }
QString DTOMessage::getMessageNickname() const { return message_nickname; }
QString DTOMessage::getMessageText() const { return message_text; }
bool DTOMessage::getRTL() const { return is_rtl; }
listLikes DTOMessage::getMessageListLike() const { return message_list_likes; }
QString DTOMessage::getMessageImageId() const { return message_image_id; }
void DTOMessage::setMessageId(const QString& message_id_) { message_id = message_id_; }
void DTOMessage::setMessageNickname(const QString& message_nickname_) { message_nickname = message_nickname_; }
void DTOMessage::setMessageText(const QString& message_text_) { message_text = message_text_; }
void DTOMessage::setMessageListLikes(const listLikes& message_list_likes_) { message_list_likes = message_list_likes_; }
void DTOMessage::setMessageImageId(const QString& message_image_id_) { message_image_id = message_image_id_; }

QSharedPointer<MessageItem> DTOMessage::createMessageItemFromDTO(const DTOMessage& dto_message_) {
	QSharedPointer<MessageItem> shp_message_item = QSharedPointer<MessageItem>::create(
		dto_message_.getMessageId(), dto_message_.getMessageNickname(), dto_message_.getMessageText(),
		dto_message_.getRTL(), dto_message_.getMessageListLike(), dto_message_.getMessageImageId());
	return shp_message_item;
}

QSharedPointer<DTOMessage> DTOMessage::createDTOMessageFromMessageItem(const MessageItem& message_item_) {
	QSharedPointer<DTOMessage> shp_dto_message = QSharedPointer<DTOMessage>::create(
		message_item_.getMesId(), message_item_.getMesNickname(), message_item_.getMesText(),
		message_item_.isRtl(), message_item_.getMesListLikes(), message_item_.getMesMediaId());
	return shp_dto_message;
}