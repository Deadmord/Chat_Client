#pragma once

#include <QWidget>
#include <QListView>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>


#include "entities.h"
#include "LogInWModel.h"


class LogInW : public QWidget
{
	Q_OBJECT;
public:
	QListView* logIn_listView;
	QPushButton* log_in_button;
	QPushButton* sign_in_button;
	QLineEdit* nickname_edit;
	QLineEdit* password_edit;

	LogInWModel* logIn_model;

public:
	LogInW(QWidget* parent = 0);
	~LogInW();

public slots:
	void insertValueFromLogInForm();


};

