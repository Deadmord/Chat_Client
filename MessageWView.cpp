#include "MessageWView.h"

MessageWView::MessageWView(QWidget* parent)
	: QListView(parent)
{
	setFont(MESSAGE_FONT);

	QTimer::singleShot(100, [this]
		{
			const auto model = new MessageWModel(this);
			setModel(model);
			setItemDelegate(new MessageWDelegate(this));
			connect(model, &MessageWModel::rowsInserted, this, &MessageWView::onRowsInserted);
		}
	);
	// important !!!
	setResizeMode(Adjust);

	setContextMenuPolicy(Qt::ActionsContextMenu);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSpacing(35);
	setMouseTracking(true);
	setStyleSheet(VIEW_STYLE_SHEET);

	connect(this, &MessageWView::customContextMenuRequested, this, &MessageWView::onCustomContextMenuRequested);
}

MessageWView::~MessageWView() = default;

void MessageWView::onMessageAdded(const QVariant& new_msg) const
{
	const auto model = qobject_cast<MessageWModel*>(this->model());
	if (!model)
		return;
	
	model->addMessage(new_msg);

}

void MessageWView::onMessagesAdded(const QVariantList& new_msg) const
{
	const auto model = qobject_cast<MessageWModel*>(this->model());
	if (!model)
		return;
	model->addMessages(new_msg);
}

void MessageWView::onCustomContextMenuRequested(const QPoint& pos)
{
	QModelIndexList indexes = selectionModel()->selectedIndexes();
	if (!indexes.isEmpty())
	{
		auto msg = indexes.front().data(MessageWModel::MessageRole).value< messageItemPtr>();
		QMenu menu;
		menu.addAction("Action...", [this, msg]
			{
				//QMessageBox::information(this, "Action", QString("Action for message: %1").arg(msg->get_msg_id()));
			});
		menu.exec(mapToGlobal(pos));
	}
}

void MessageWView::onRowsInserted(const QModelIndex& parent, int first, int last)
{
	ensurePolished();
	scrollToBottom();
}

void MessageWView::mouseMoveEvent(QMouseEvent* event)
{
	if (const auto& model = qobject_cast<MessageWModel*>(this->model()))
	{
		auto const& item_index = indexAt(event->pos());
		if (_last_index != item_index)
		{
			if (_last_index.isValid())
			{
				if (const auto& msg = _last_index.data(MessageWModel::MessageRole).value< messageItemPtr>())
				{
					msg->setCurrentRow(_last_index.row());
					msg->setIsHovered(false);
					emit model->dataChanged(_last_index, _last_index);
				}
			}
		}
		_last_index = item_index;
		if (_last_index.isValid())
		{
			const auto msg = _last_index.data(MessageWModel::MessageRole).value< messageItemPtr>();
			msg->setIsHovered(true);
			emit model->dataChanged(_last_index, _last_index);
		}
	}
	QListView::mouseMoveEvent(event);
}

void MessageWView::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (const auto& model = qobject_cast<MessageWModel*>(this->model()))
	{
		auto const& item_index = indexAt(event->pos());
		if (item_index.isValid())
		{
			const auto& msg = item_index.data(MessageWModel::MessageRole).value< messageItemPtr>();
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
	QListView::mouseDoubleClickEvent(event);
}

void MessageWView::mouseReleaseEvent(QMouseEvent* event)
{
	if (const auto& model = qobject_cast<MessageWModel*>(this->model()))
	{
		auto const& item_index = indexAt(event->pos());
		if (item_index.isValid())
		{
			const auto& msg = item_index.data(MessageWModel::MessageRole).value< messageItemPtr>();
			if (msg->getLikeButtRect().contains(event->pos()))
			{
				msg->changeMesLikes(1);
				emit model->dataChanged(item_index, item_index);
			}
			else if (msg->getDislikeButtRect().contains(event->pos()))
			{
				msg->changeMesLikes(-1);
				emit model->dataChanged(item_index, item_index);
			}
		}
	}
	QListView::mouseReleaseEvent(event);
}

