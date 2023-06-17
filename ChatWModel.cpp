#include "ChatWModel.h"

ChatWModel::ChatWModel(QObject* parent)
	: QAbstractListModel(parent)
{}

ChatWModel::~ChatWModel()
{
	clear();
}

void ChatWModel::clear()
{
    beginResetModel();
    model_chats.clear();
    endResetModel();
}

void ChatWModel::addChat(const QVariant& new_chat_var_)
{
    if (const auto msg = new_chat_var_.value<chatItemPtr>(); msg)
    {
        beginInsertRows(QModelIndex(), static_cast<int>(model_chats.size()), static_cast<int>(model_chats.size()));
        model_chats.emplaceBack(msg);
        endInsertRows();
    }
}

void ChatWModel::addChats(const QVariantList& new_chat_list_)
{
    if (!new_chat_list_.isEmpty())
    {
        beginInsertRows(QModelIndex(), static_cast<int>(model_chats.size()), static_cast<int>(model_chats.size() + new_chat_list_.size() - 1));
        for (auto const& chat_var : new_chat_list_)
        {
            if (const auto msg = chat_var.value<chatItemPtr>(); msg)
            {
                model_chats.emplaceBack(msg);
            }
        }
        endInsertRows();
    }
}

bool ChatWModel::hashChat(const std::bitset<128>& hash) const
{
    return model_hash_to_chat.contains(hash);
}

chatItemPtr ChatWModel::getChatFromId(const int id) const
{
    return model_id_to_chat.value(id);
}

int ChatWModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
        return static_cast<int>(model_chats.size());
}

QVariant ChatWModel::data(const QModelIndex& index, int role) const
{
    if (auto const chat = (index.isValid() && index.row() < rowCount()) ? model_chats.at(index.row()) : nullptr)
    {
        switch (role)
        {
        case Qt::DisplayRole:
            return chat->getChatRoomName();

        case Qt::SizeHintRole:
            return QSize{ 200, 200 };

        case ChatRole:
            return QVariant::fromValue(chat);

        default:
            break;
        }
    }

    return {};
}

Qt::ItemFlags ChatWModel::flags(const QModelIndex& index) const
{
    return QAbstractListModel::flags(index);
}

