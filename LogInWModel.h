#pragma once

#include <QAbstractListModel>
#include <plog/Log.h> 

#include "entities.h"

class LogInWModel : public QAbstractListModel
{
public:
    LogInWModel(QObject* parent = nullptr)
        : QAbstractListModel(parent)
    {
    }
    void setLogInData(const QList<LogInWStruct>& chatData);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    QList<LogInWStruct> logIn_list_model;

public slots:
    void onLogInClick(const LogInWStruct& value_);
    void onSignInClick();
};

