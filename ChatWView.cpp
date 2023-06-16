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

	/////  Test for 1M messages !!!
	//	QVariantList msg_list;
	//	for ( int i = 0; i < 1000000; ++i )
	//	{
	//		msg_list << new_msg;
	//	}
	//	model->add_messages(msg_list);

	model->addChat(new_msg);
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
	/*if (const auto& model = qobject_cast<ChatWModel*>(this->model()))
	{
		auto const& item_index = indexAt(event->pos());
		if (item_index.isValid())
		{
			const auto& msg = item_index.data(ChatWModel::ChatRole).value< chatItemPtr>();
			const auto& image_rects = msg->getImageBoxRects();
			int index = -1;
			for (int i = 0; i < image_rects.size(); ++i)
			{
				if (image_rects.at(i).contains(event->pos()))
				{
					index = i;
					break;
				}
			}
			if (index >= 0)
			{
				Q_EMIT imageClicked(msg->getMesFilelist().at(index));
			}
		}
	}
	QListView::mouseDoubleClickEvent(event);*/
}

void ChatWView::mouseReleaseEvent(QMouseEvent* event)
{
	if (const auto& model = qobject_cast<ChatWModel*>(this->model()))
	{
		auto const& item_index = indexAt(event->pos());
		if (item_index.isValid())
		{
			const auto& chat = item_index.data(ChatWModel::ChatRole).value<chatItemPtr>();
			/*if (msg->getLikeButtRect().contains(event->pos()))
			{
				msg->changeMesLikes(1);
				emit model->dataChanged(item_index, item_index);
			}
			else if (msg->getDislikeButtRect().contains(event->pos()))
			{
				msg->changeMesLikes(-1);
				emit model->dataChanged(item_index, item_index);
			}*/
		}
	}
	QListView::mouseReleaseEvent(event);
}

