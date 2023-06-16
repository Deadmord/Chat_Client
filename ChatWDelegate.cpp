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
	const QFontMetrics fm{ CHAT_FONT };

	//Lable rect
	const QFontMetrics fm_l{ CHAT_LABLE_FONT };
	const int lable_width = fm_l.horizontalAdvance(chat->getChatRoomTopicName(), chat->getChatRoomTopicName().size());
	QRect lable_rect = body_rect.adjusted(body_rect.width() - 10 - lable_width, 10, -10, -10);

	//Descr rect
	const QFontMetrics fm_d{ CHAT_DESCRIPTION_FONT };
	const int name_width = fm.horizontalAdvance(chat->getChatRoomName(), chat->getChatRoomName().size());

	const int max_descr_width = body_rect.width() / 2;
	QString text_descr = chat->getChatRoomDescription();

	int lines = 1;
	int descr_width = fm_d.horizontalAdvance(text_descr, text_descr.size());
	if (max_descr_width < descr_width) {
		QString currentLine;

		for (int i = 0; i < text_descr.size(); i++) {
			QString newLine = currentLine + text_descr[i];
			int lineWidth = fm_d.horizontalAdvance(newLine);

			if (lineWidth <= max_descr_width) {
				// The new character fits within the maximum width
				currentLine = newLine;
			}
			else {
				// Start a new line
				text_descr.insert(i, QString("\n"));
				lines++;
				currentLine.clear();
			}
		}

		descr_width = max_descr_width < descr_width ? max_descr_width : descr_width;
	}
	int height = lines == 0 ? fm_d.height() : fm_d.height() * lines;
	QRect descr_rect = body_rect.adjusted(10 + name_width + 10, 2, -body_rect.width() + 20 + name_width + descr_width, -body_rect.height() + height + 2);
	auto a = 5;

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

	//draw lable
	auto const draw_lable = [&]()
	{
		painter->setBrush(LABLE_COLOR);

		// Draw the rectangle with the background color
		painter->drawRect(lable_rect);
		painter->setFont(QFont(CHAT_LABLE_FONT));
		painter->drawText(lable_rect, Qt::AlignLeft | Qt::AlignCenter, chat->getChatRoomTopicName());
	};

	//draw descr
	auto const draw_descr = [&]()
	{
		if (hovered_chat) {
			painter->setBrush(DESCRIPTION_COLOR);
			painter->setPen(Qt::NoPen);
			// Draw the rectangle with the background color
			painter->drawRect(descr_rect);
			painter->setFont(QFont(CHAT_DESCRIPTION_FONT));
			painter->setPen(Qt::black);

			painter->drawText(descr_rect, Qt::AlignLeft | Qt::AlignCenter, text_descr);
		}
	};


	draw_chatname();
	draw_lable();
	draw_descr();
	painter->restore();
};