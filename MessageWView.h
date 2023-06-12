#pragma once

#include <QListView>
#include <QTimer>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>

#include "Styles.h"
#include "MessageItem.h"
#include "MessageWModel.h"
#include "MessageWDelegate.h"

class MessageWView : public QListView
{
	Q_OBJECT;

public:
	explicit MessageWView(QWidget* parent);
	~MessageWView() override;

	MessageWView(const MessageWView&) = delete;
	MessageWView(MessageWView&&) = delete;
	const MessageWView& operator =(const MessageWView&) = delete;
	MessageWView& operator = (MessageWView&&) = delete;

public Q_SLOTS:
	void onMessageAdded(const QVariant& new_msg) const;
	void onCustomContextMenuRequested(const QPoint& pos);
	void onRowsInserted(const QModelIndex& parent, int first, int last);

Q_SIGNALS:
	void imageClicked(const QString& image_path);

protected:
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	QModelIndex _last_index{};
};

