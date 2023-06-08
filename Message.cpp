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

	//QWidget* editorWidget = new QWidget(chat_lineEdit);

	//// Create the buttons
	//QPushButton* button1 = new QPushButton("Button 1", editorWidget);
	//QPushButton* button2 = new QPushButton("Button 2", editorWidget);

	// Set the layout for the widget
	/*QHBoxLayout* butt_layout = new QHBoxLayout(editorWidget);
	butt_layout->addWidget(button1);
	butt_layout->addWidget(button2);
	butt_layout->setContentsMargins(0, 0, 0, 0);*/

	

	//QWidget* editorWidget = new QWidget(parent);
	/*QPushButton* button1 = new QPushButton("Button 1", chat_listVeiw);
	QPushButton* button2 = new QPushButton("Button 2", chat_listVeiw);*/

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