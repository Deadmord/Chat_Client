#pragma once
#include <QWidget>
#include <QListView>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>

#include "MessageModel.h"
#include "MessageDelegate.h"



class Message : public QWidget
{
	Q_OBJECT;

	QListView* chat_listVeiw;
	QPushButton* chat_buttonSend;
	QLineEdit* chat_lineEdit;
	QListView* chat_m_messageVeiw;
	//QPushButton* chat_m_button_like;
	//QPushButton* chat_m_button_dislike;

	MessageModel* chat_message_model;
	MessageDelegate* chat_message_delegate;

public:
	Message(QWidget* parent = 0);
	~Message();

public slots:
	void insertValueFromEdit();

};

