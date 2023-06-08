#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QListView>
#include <QPushButton>
#include <QPainterPath>
#include "QHBoxLayout"
#include "entities.h"
#include "MyFont.h"
#include "paint_defs.h"


class MessageDelegate : public QStyledItemDelegate {

//public:
//	explicit MessageDelegate(QObject* parent);
//	~MessageDelegate() override;
//	MessageDelegate(const MessageDelegate&) = delete;
//	MessageDelegate(MessageDelegate&&) = delete;
//	const MessageDelegate& operator =(const MessageDelegate&) = delete;
//	MessageDelegate& operator = (MessageDelegate&&) = delete;
//
//protected:
//
//	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
//	[[nodiscard]] QSize sizeHint(QStyleOptionViewItem const& option, QModelIndex const& index) const override;

public:
    explicit MessageDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent)
    {
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    const QString font = "Ariel";
    const int nickname_size = 12;
    const int text_size = 10;
    const int time_size = 5;

    MyFont text_font = MyFont(font, nickname_size);
    MyFont nickname_font = MyFont(font, text_size);
    MyFont time_font = MyFont(font, time_size);

	QPixmap _like_pixmap{ ":/mainWindow/resources/heart.png" };
	QPixmap _dislike_pixmap{ ":/mainWindow/resources/broken.png" };
};

