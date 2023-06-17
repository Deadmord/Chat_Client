#pragma once

#include <QListView>
#include <QTimer>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>

#include "Styles.h"
#include "ChatWModel.h"
#include "ChatWDelegate.h"

class ChatWView : public QListView
{
	Q_OBJECT;

public:
	explicit ChatWView(QWidget* parent);
	~ChatWView() override;

	ChatWView(const ChatWView&) = delete;
	ChatWView(ChatWView&&) = delete;
	const ChatWView& operator =(const ChatWView&) = delete;
	ChatWView& operator = (ChatWView&&) = delete;

public Q_SLOTS:
	void onChatAdded(const QVariant& new_msg) const;
	void onChatsAdded(const QVariantList& msg_list_) const;
	void onCustomContextMenuRequested(const QPoint& pos);
	void onRowsInserted(const QModelIndex& parent, int first, int last);

Q_SIGNALS:
	void chatClicked(qint32 chat_id_);

protected:
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	//void mouseReleaseEvent(QMouseEvent* event) override;

private:
	QModelIndex _last_index{};
};

