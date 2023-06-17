#pragma once

#include <QItemDelegate>
#include <QPen>
#include <QPainter>

#include "ChatItem.h"
#include "ChatWModel.h"
#include "Styles.h"

class ChatItem;
class ChatWDelegate final : public QItemDelegate //ma ze final
{
	Q_OBJECT

public:
	explicit ChatWDelegate(QObject* parent);
	~ChatWDelegate() override;
	ChatWDelegate(const ChatWDelegate&) = delete;
	ChatWDelegate(ChatWDelegate&&) = delete;
	const ChatWDelegate& operator =(const ChatWDelegate&) = delete;
	ChatWDelegate& operator = (ChatWDelegate&&) = delete;

protected:

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	[[nodiscard]] QSize sizeHint(QStyleOptionViewItem const& option, QModelIndex const& index) const override;
};

