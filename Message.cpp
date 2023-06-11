#include "Message.h"


Message::Message(QWidget* parent) : QWidget(parent)
{
	chat_message_model = new MessageModel(this);
	chat_message_delegate = new MessageDelegate(this);
	chat_listVeiw = new QListView(this);
	chat_listVeiw->setModel(chat_message_model);
	chat_listVeiw->setItemDelegate(chat_message_delegate);

	chat_lineEdit = new QLineEdit(this);
	chat_buttonSend = new QPushButton("Send", this);

	setLayout(new QVBoxLayout);

	QHBoxLayout* chat_lay = new QHBoxLayout(this);
	chat_lay->addWidget(chat_lineEdit);
	chat_lay->addWidget(chat_buttonSend);

	layout()->addWidget(chat_listVeiw);
	layout()->addItem(chat_lay);

	connect(chat_buttonSend, SIGNAL(clicked(bool)), this, SLOT(insertValueFromEdit()));
	//connect(); - For error messages
}

Message::~Message()
{
	close();
}

void Message::insertValueFromEdit() {
	chat_message_model->sendMessage(chat_lineEdit->text());
}