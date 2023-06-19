#include "DTOMessage.h"
DTOMessage::DTOMessage() {}
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

bool DTOMessage::toDTOMessageFromJson(DTOMessage& user_masg_dto_, const QJsonObject& user_msg_)
{
    const QJsonValue id_val = user_msg_.value(QLatin1String("id"));
    if (id_val.isNull() || !id_val.isString())
        return false;
    const QString id = id_val.toString().trimmed();
    if (id.isEmpty())
        return false;

    const QJsonValue parentid_val = user_msg_.value(QLatin1String("parentid"));
    if (parentid_val.isNull() || !parentid_val.isString())
        return false;
    const QString parentid = parentid_val.toString().trimmed();

    const QJsonValue date_time_val = user_msg_.value(QLatin1String("datetime"));
    if (date_time_val.isNull() || !date_time_val.isString())
        return false;
    const QString date_time = date_time_val.toString().trimmed();
    if (date_time.isEmpty())
        return false;

    const QJsonValue nickname_val = user_msg_.value(QLatin1String("nickname"));
    if (nickname_val.isNull() || !nickname_val.isString())
        return false;
    const QString nickname = nickname_val.toString().trimmed();
    if (nickname.isEmpty())
        return false;

    const QJsonValue text_val = user_msg_.value(QLatin1String("text"));
    if (text_val.isNull() || !text_val.isString())
        return false;
    const QString text = text_val.toString().trimmed();
    if (text.isEmpty())
        return false;

    const QJsonValue mediaid_val = user_msg_.value(QLatin1String("mediaid"));
    if (mediaid_val.isNull() || !mediaid_val.isString())
        return false;
    const QString mediaid = mediaid_val.toString().trimmed();

    const QJsonValue rtl_val = user_msg_.value(QLatin1String("rtl"));
    if (rtl_val.isNull() || !rtl_val.isBool())
        return false;
    const bool rtl = rtl_val.toBool();


    const auto likes_val = user_msg_.value(QLatin1String("likes")).toObject().toVariantMap();
    QMap<QString, bool> likes;
    for (auto it = likes_val.constBegin(); it != likes_val.constEnd(); ++it)
    {
        likes.insert(it.key(), it.value().toBool());
    }

    user_masg_dto_.setMessageId(id);
    user_masg_dto_.setMessageNickname(nickname);
    user_masg_dto_.setMessageText(text);
    user_masg_dto_.setMessageImageId(mediaid);
    return true;
}

bool DTOMessage::toJsonFromDTOMessage(QJsonObject& user_msg_, const DTOMessage& user_masg_dto_)
{
    return false;
}