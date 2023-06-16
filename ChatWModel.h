#pragma once

#include <QAbstractListModel>
#include <bitset>
#include <QSize>

//#include "ChatWView.h"
#include "ChatItem.h"

class ChatWModel : public QAbstractListModel
{
	Q_OBJECT;
	friend class ChatWView;

public:
	enum Role
	{
		ChatRole = Qt::UserRole,
	};

	explicit ChatWModel(QObject* parent = Q_NULLPTR);
	~ChatWModel() override;
	ChatWModel(const ChatWModel&) = delete;
	ChatWModel(ChatWModel&&) = delete;
	const ChatWModel& operator =(const ChatWModel&) = delete;
	ChatWModel& operator = (ChatWModel&&) = delete;

	[[nodiscard]] bool hashChat(const std::bitset<128>& hash) const; //ma ze??

	[[nodiscard]] chatItemPtr getChatFromId(const int id) const;

	[[nodiscard]] int rowCount(QModelIndex const& parent = QModelIndex()) const override final;
	[[nodiscard]] QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override final;
	[[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override final;

	using QAbstractListModel::beginInsertRows;
	using QAbstractListModel::endInsertRows;
	using QAbstractListModel::beginRemoveRows;
	using QAbstractListModel::endRemoveRows;
	using QAbstractListModel::beginResetModel;
	using QAbstractListModel::endResetModel;

	using QAbstractListModel::dataChanged;

	public Q_SLOT:
	void addChat(const QVariant& new_chat_var_);
private Q_SLOTS:

	void clear();
	void addChats(const QVariantList& new_chat_list_);

private:

	chatList model_chats;
	QMap<uint64_t, chatItemPtr> model_id_to_chat;
	QHash<std::bitset<128>, chatItemPtr> model_hash_to_chat;
};


