#include "LogInWModel.h"

void LogInWModel::setLogInData(const QList<LogInWStruct>& chatData_)
{
    beginResetModel();
    logIn_list_model = chatData_;
    endResetModel();
}

int LogInWModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return logIn_list_model.size();
}

QVariant LogInWModel::data(const QModelIndex& index_, int role_) const
{
    if (!index_.isValid())
        return QVariant();

    if (role_ == Qt::DisplayRole) {
        const LogInWStruct& logIn = logIn_list_model.at(index_.row());
        return QVariant::fromValue(logIn);
    }

    return QVariant();
}

void LogInWModel::onSignInClick()
{
}

void LogInWModel::onLogInClick(const LogInWStruct& value_) {
    //TODO make send request for server. Waiting for responce. Hoe to paint loading picture?
    PLOGD << "Data comes when you click on button" << value_.log_in_nickname << " " << value_.log_in_password;
    logIn_list_model.clear();
    emit layoutChanged();
}
