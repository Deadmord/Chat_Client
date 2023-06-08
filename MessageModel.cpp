#include "MessageModel.h"



void MessageModel::setChatData(const QList<MessageStruct>& chatData)
{
    beginResetModel();
    message_list_model = chatData;
    endResetModel();
}

int MessageModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return message_list_model.size();
}

QVariant MessageModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        const MessageStruct& message = message_list_model.at(index.row());
        return QVariant::fromValue(message);
    }

    return QVariant();
}

void MessageModel::sendMessage(const QString& value_) {
    //message_list_model.append(value_);
    message_list_model.append({ "Nuta", value_, "1", QDateTime::currentDateTime(QTimeZone::systemTimeZone()) });
    emit layoutChanged();
}
