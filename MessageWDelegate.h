#pragma once

#include <QItemDelegate>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsBlurEffect>

#include "MessageItem.h"
#include "MessageWModel.h"

class MessageItem;
class MessageWDelegate final : public QItemDelegate
{
	Q_OBJECT

public:
	explicit MessageWDelegate(QObject* parent);
	~MessageWDelegate() override;
	MessageWDelegate(const MessageWDelegate&) = delete;
	MessageWDelegate(MessageWDelegate&&) = delete;
	const MessageWDelegate& operator =(const MessageWDelegate&) = delete;
	MessageWDelegate& operator = (MessageWDelegate&&) = delete;

protected:

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	[[nodiscard]] QSize sizeHint(QStyleOptionViewItem const& option, QModelIndex const& index) const override;


private:

	QLinearGradient* _msg_box_gradient = new QLinearGradient{ 0, 0, 0, 100 };
	QPixmap _like_pixmap{ ":/ChatClient/images/heart.png" };
	QPixmap _dislike_pixmap{ ":/ChatClient/images/broken.png" };

};

