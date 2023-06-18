#pragma once

#include <QEnableSharedFromThis>
#include <QObject>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <plog/Log.h> 

#include "UserItem.h"




class ConfigData{

public:
    static ConfigData getConfig() {
        ConfigData config;
        return config;
    }

    void saveConfig(const QString& nickname_, const QString& password_) {
        QFile           config_file;
        QJsonObject     config_json;
        QJsonParseError jsonError;

        config_file.setFileName(CONFIG_FILE_PATH);
        if (config_file.open(QIODevice::WriteOnly | QFile::Text))
        {
            QJsonObject json;
            QJsonObject user;

            json["ServerAddress"] = config_server_adress;
            json["ServerPort"] = config_server_port;
            user["Nickname"] = nickname_;
            user["Password"] = password_;
            json["User"] = user;
            config_json = json;
            config_file.write(QJsonDocument(config_json).toJson());
        }
        else
        {
            qDebug() << "Error configuration file cannot be opened.";
        }
        config_file.close();
    };


    auto getConfNickname() const { return config_user_nickname; };
    auto setConfNickanme(const QString& var_) { config_user_nickname = var_; };
    auto getConfPassword() const { return config_user_password; };
    auto setConfPassword(const QString& var_) { config_user_password = var_; };

    auto getConfPort() { return config_server_port; };
    auto getConfServer() { return config_server_adress;  };

private:
    explicit ConfigData()
    {
        QFile           config_file;
        QJsonParseError jsonError;
        QJsonDocument   config_file_doc;

        config_file.setFileName(CONFIG_FILE_PATH);
        if (config_file.open(QIODevice::ReadOnly | QFile::Text))
        {
            config_file_doc = QJsonDocument::fromJson(QByteArray(config_file.readAll()), &jsonError);
            config_file.close();

            if (jsonError.error == QJsonParseError::NoError)
            {
                QJsonObject config_json = config_file_doc.object();
                if (const QJsonValue v = config_file_doc["User"]["Nickname"]; v.isString())
                    config_user_nickname = v.toString();
                else
                {
                    config_user_nickname = "";
                    PLOGE << "Error nickname reading";
                }

                if (const QJsonValue v = config_file_doc["User"]["Password"]; v.isString())
                    config_user_password = v.toString();
                else
                {
                    config_user_password = "";
                    PLOGE << "Error password reading";
                }
            }
            else
            {
                PLOGE << "Error config file read: " << jsonError.error;
            }
        }
        else
        {
            PLOGW << "Configuration file can't be open (not exist)";
        }
    };



private:

private:
    const QString	CONFIG_FILE_PATH = "./config.json";

    const QString   config_server_adress = "127.0.0.1";
    const quint16   config_server_port = 5555;
    QString	        config_user_nickname;
    QString			config_user_password;

    void loadConfig() {}

};

