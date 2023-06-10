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

	QPixmap _like_pixmap{ ":../Chat_Client/resources/heart.png" };
	QPixmap _dislike_pixmap{ ":../Chat_Client/resources/broken.png" };
};

