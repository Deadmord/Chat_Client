#include "ChatWView.h"


ChatWView::ChatWView(QWidget* parent)
	: QListView(parent)
{
	setFont(CHAT_FONT);

	QTimer::singleShot(100, [this]
		{
			const auto model = new ChatWModel(this);
			setModel(model);
			setItemDelegate(new ChatWDelegate(this));
			connect(model, &ChatWModel::rowsInserted, this, &ChatWView::onRowsInserted);
		}
	);
	// important !!!
	setResizeMode(Adjust);

	setContextMenuPolicy(Qt::ActionsContextMenu);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSpacing(0);
	setMouseTracking(true);
	setStyleSheet(VIEW_CHAT_STYLE_SHEET);

	connect(this, &ChatWView::customContextMenuRequested, this, &ChatWView::onCustomContextMenuRequested);
}

ChatWView::~ChatWView() = default;

void ChatWView::onChatAdded(const QVariant& new_msg) const
{
	const auto model = qobject_cast<ChatWModel*>(this->model());
	if (!model)
		return;
	model->addChat(new_msg);
}

void ChatWView::onChatsAdded(const QVariantList& msg_list) const
{
	const auto model = qobject_cast<ChatWModel*>(this->model());
	if (!model)
		return;
	model->addChats(msg_list);
}

void ChatWView::onCustomContextMenuRequested(const QPoint& pos)
{
	QModelIndexList indexes = selectionModel()->selectedIndexes();
	if (!indexes.isEmpty())
	{
		auto chat = indexes.front().data(ChatWModel::ChatRole).value<chatItemPtr>();
		QMenu menu;
		menu.addAction("Action...", [this, chat]
			{
				//QMessageBox::information(this, "Action", QString("Action for message: %1").arg(msg->get_msg_id()));
			});
		menu.exec(mapToGlobal(pos));
	}
}

void ChatWView::onRowsInserted(const QModelIndex& parent, int first, int last)
{
	ensurePolished();
	scrollToBottom();
}

void ChatWView::mouseMoveEvent(QMouseEvent* event)
{
	if (const auto& model = qobject_cast<ChatWModel*>(this->model()))
	{
		auto const& item_index = indexAt(event->pos());
		if (_last_index != item_index)
		{
			if (_last_index.isValid())
			{
				if (const auto& chat = _last_index.data(ChatWModel::ChatRole).value<chatItemPtr>())
				{
					chat->setChatRoomCurrRow(_last_index.row());
					chat->setIsHovered(false);
					emit model->dataChanged(_last_index, _last_index);
				}
			}
		}
		_last_index = item_index;
		if (_last_index.isValid())
		{
			const auto msg = _last_index.data(ChatWModel::ChatRole).value<chatItemPtr>();
			msg->setIsHovered(true);
			emit model->dataChanged(_last_index, _last_index);
		}
	}
	QListView::mouseMoveEvent(event);
}

void ChatWView::mouseDoubleClickEvent(QMouseEvent* event)
{	
	if (const auto& model = qobject_cast<ChatWModel*>(this->model()))
	{
		auto const& item_index = indexAt(event->pos());
		if (item_index.isValid())
		{
			const auto& chat = item_index.data(ChatWModel::ChatRole).value< chatItemPtr>();
			const auto& chat_rects = chat->getChatRoomCurrBox();
			if (chat_rects.contains(event->pos()))
			{
				Q_EMIT chatClicked(chat->getChatId(), chat->getChatRoomName());
			}
		}
	}
	QListView::mouseDoubleClickEvent(event);
}

