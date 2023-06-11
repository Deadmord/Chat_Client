#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QListView>

#include "entities.h"
#include "MyFont.h"


class LogInWDelegate : public QStyledItemDelegate
{
public:
    explicit LogInWDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent)
    {
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    const QString font = "Ariel";
    const int data_size = 12;

    MyFont data_font = MyFont(font, data_size);
};

