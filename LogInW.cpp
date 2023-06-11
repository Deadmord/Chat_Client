#include "LogInW.h"


LogInW::LogInW(QWidget* parent) : QWidget(parent)
{
	logIn_model = new LogInWModel(this);

	logIn_listView = new QListView(this);
	logIn_listView->setModel(logIn_model);

	nickname_edit = new QLineEdit(this);
	password_edit = new QLineEdit(this);
	log_in_button = new QPushButton("Log In", this);
	sign_in_button = new QPushButton("Sign In", this);

	setLayout(new QVBoxLayout);

	QHBoxLayout* buttons_lay = new QHBoxLayout(this);
	buttons_lay->addWidget(log_in_button);
	buttons_lay->addWidget(sign_in_button);

	layout()->addWidget(nickname_edit);
	layout()->addWidget(password_edit);
	layout()->addItem(buttons_lay);

	QString styleSheet = "QWidget { border-radius: 12px; background-color: #4CAF50;box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2), 0 6px 20px 0 rgba(0,0,0,0.19); }";
	log_in_button->setStyleSheet(styleSheet);

	connect(log_in_button, SIGNAL(clicked(bool)), this, SLOT(insertValueFromLogInForm()));
	//TODO connect(); - For SignIn
}

LogInW::~LogInW()
{
	close();
}

void LogInW::insertValueFromLogInForm() {
	//logIn_model->onLogInClick({ nickname_edit->text() ,password_edit->text() });
	
	//TODO Sending to server
	nickname_edit->clear();
	password_edit->clear();
}