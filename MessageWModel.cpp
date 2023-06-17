#include "MessageWModel.h"

struct messageIdLessThan
{
    /*bool operator()(const messageItemPtr& left, const messageItemPtr& right) const
    {
        return left->getMsgId() < right->getMsgId();
    }

    bool operator()(const uint64_t left_id, const messageItemPtr& right) const
    {
        return left_id < right->get_msg_id();
    }

    bool operator()(const messageItemPtr& left, const uint64_t right_id) const
    {
        return left->get_msg_id() < right_id;
    }*/
};

MessageWModel::MessageWModel(QObject* parent)
    : QAbstractListModel(parent)
{}

MessageWModel::~MessageWModel()
{
    clear();
}

void MessageWModel::clear()
{
    beginResetModel();
    model_messages.clear();
    //_id_to_msg.clear();
    endResetModel();
}

void MessageWModel::addMessage(const QVariant& new_msg_var)
{
    if (const auto msg = new_msg_var.value<messageItemPtr>(); msg)
    {
        beginInsertRows(QModelIndex(), static_cast<int>(model_messages.size()), static_cast<int>(model_messages.size()));
        model_messages.emplaceBack(msg);
        //_id_to_msg.insert(msg->getMesId(), msg);
        endInsertRows();
    }
}

void MessageWModel::addMessages(const QVariantList& new_msg_list)
{
    if (!new_msg_list.isEmpty())
    {
        beginInsertRows(QModelIndex(), static_cast<int>(model_messages.size()), static_cast<int>(model_messages.size() + new_msg_list.size() - 1));
        for (auto const& msg_var : new_msg_list)
        {
            if (const auto msg = msg_var.value<messageItemPtr>(); msg)
            {
                model_messages.emplaceBack(msg);
                //_id_to_msg.insert(msg->getMesId(), msg);
            }
        }
        endInsertRows();
    }
}

bool MessageWModel::hashMessage(const std::bitset<128>& hash) const
{
    return model_hash_to_msg.contains(hash);
}

messageItemPtr MessageWModel::getMessageFromId(const int id) const
{
    return model_id_to_msg.value(id);
}

void MessageWModel::onAnotheLikeChanged(const QVariant& like_) {

    const auto like = like_.value<likeItemPtr>();
    for (const messageItemPtr& msg_ : model_messages) {
        if (like->getIdChat() == msg_->getMesId()) {
            if (like->getReaction() == Like_enum::LIKE) {
                msg_->changeMesLikes(1);
            }
            else {
                msg_->changeMesLikes(-1);
            }
            msg_->addNewLike(like);
        }
    }
}


int MessageWModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
        return static_cast<int>(model_messages.size());
}

QVariant MessageWModel::data(const QModelIndex& index, int role) const
{
    if (auto const msg = (index.isValid() && index.row() < rowCount()) ? model_messages.at(index.row()) : nullptr)
    {
        switch (role)
        {
        case Qt::DisplayRole:
            return msg->getMesText();

        case Qt::SizeHintRole:
            return QSize{ 200, 200 };

        case Qt::DecorationRole:
            return msg->getMesAvatar();

        case MessageRole:
            return QVariant::fromValue(msg);

        default:
            break;
        }
    }

    return {};
}

Qt::ItemFlags MessageWModel::flags(const QModelIndex& index) const
{
    return QAbstractListModel::flags(index);
}
