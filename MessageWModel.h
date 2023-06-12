#pragma once

#include <QAbstractListModel>
#include <bitset>
#include "messageitem.h"

class MessageWModel final : public QAbstractListModel
{
	Q_OBJECT

		friend class messageView;

public:
	enum Role
	{
		MessageRole = Qt::UserRole,
	};

	explicit MessageWModel(QObject* parent = Q_NULLPTR);
	~MessageWModel() override;
	MessageWModel(const MessageWModel&) = delete;
	MessageWModel(MessageWModel&&) = delete;
	const MessageWModel& operator =(const MessageWModel&) = delete;
	MessageWModel& operator = (MessageWModel&&) = delete;

	[[nodiscard]] bool hashMessage(const std::bitset<128>& hash) const;

	[[nodiscard]] messageItemPtr getMessageFromId(const int id) const;

	[[nodiscard]] int rowCount(QModelIndex const& parent = QModelIndex()) const override final;
	[[nodiscard]] QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override final;
	[[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override final;

	//void update_raw( const int i_row);
	//void update_all();

	using QAbstractListModel::beginInsertRows;
	using QAbstractListModel::endInsertRows;
	using QAbstractListModel::beginRemoveRows;
	using QAbstractListModel::endRemoveRows;
	using QAbstractListModel::beginResetModel;
	using QAbstractListModel::endResetModel;

	using QAbstractListModel::dataChanged;

public Q_SLOT:
	void addMessage(const QVariant& new_msg_var_);
private Q_SLOTS:

	void clear();
	void addMessages(const QVariantList& new_msg_list_);

private:

	messagesList model_messages;
	QMap<uint64_t, messageItemPtr> model_id_to_msg;
	QHash<std::bitset<128>, messageItemPtr> model_hash_to_msg;

};


