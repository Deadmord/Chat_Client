#include "ChatWDelegate.h"


ChatWDelegate::ChatWDelegate(QObject* parent)
	: QItemDelegate(parent)
{
}

ChatWDelegate::~ChatWDelegate()
{}

QSize ChatWDelegate::sizeHint(QStyleOptionViewItem const& option, QModelIndex const& index) const
{
	const auto& chat = index.data(ChatWModel::ChatRole).value<chatItemPtr>();

	const int added_height = CONTENT_MARGINS.y() * 2;
	const QFontMetrics fm{ CHAT_FONT };
	return { option.rect.width(), fm.height() + added_height };
}

void ChatWDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const auto& chat = index.data(ChatWModel::ChatRole).value<chatItemPtr>();

	const auto private_chat = chat->getChatRoomIsPrivate();
	const auto hovered_chat = chat->isHovered();


	QRect body_rect = option.rect.adjusted(0,0,0,0);

	//Description rect
	const QFontMetrics fm{ CHAT_DESCR_FONT };
	const int descr_width = fm.horizontalAdvance(chat->getChatRoomTopicName(), chat->getChatRoomTopicName().size());
	QRect descr_rect = body_rect.adjusted(body_rect.width() - 10 - descr_width, 10, -10, -10);


	/// drawing lambdas
	auto const draw_chatname = [&]()
	{
		auto const border_color = hovered_chat ? HOVERED_CHAT_COLOR : NORMAL_CHAT_COLOR;
		painter->setBrush(border_color);

		// Draw the rectangle with the background color
		painter->drawRect(body_rect);

		//hovered_chat ? painter->setBrush(QColor(255, 0, 0)) : painter->setBrush(QColor(0, 255, 0));
		hovered_chat ? painter->setFont(QFont(CHAT_HOVER_FONT)) : painter->setFont(QFont(CHAT_FONT));
		//painter->setFont(QFont("Titilium Web", 14, QFont::Bold));

		QRect messageRect = body_rect.adjusted(CONTENT_MARGINS.x(), CONTENT_MARGINS.y(), option.rect.width(), option.rect.height());
		painter->drawText(messageRect, Qt::AlignLeft | Qt::AlignTop, chat->getChatRoomName());
	};

	//draw action
	auto const draw_lable = [&]()
	{
		if (hovered_chat) {
			painter->setBrush(DESCRIPTION_COLOR);

			// Draw the rectangle with the background color
			painter->drawRect(descr_rect);
			painter->setFont(QFont(CHAT_DESCR_FONT));
			painter->drawText(descr_rect, Qt::AlignLeft | Qt::AlignCenter, chat->getChatRoomTopicName());
		}
	};


	draw_chatname();
	draw_lable();
	painter->restore();
};