#pragma once

#include <QAbstractListModel>
#include <QTimeZone>

#include "entities.h"

class MessageModel : public QAbstractListModel
{
public:
    MessageModel(QObject* parent = nullptr)
        : QAbstractListModel(parent)
    {
    }
    void setChatData(const QList<MessageStruct>& chatData);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    QList<MessageStruct> message_list_model;

public slots:
    void sendMessage(const QString& value_);
};

