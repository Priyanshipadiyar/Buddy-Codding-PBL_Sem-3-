#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QString>
#include <QJsonObject>

class AuthManager
{
public:
    AuthManager();

    bool signup(QString name, QString email,
                QString password, QString *error = nullptr);

    bool login(QString email, QString password,
               QString *error = nullptr);

    // Login with user details
    bool loginWithUser(QString email, QString password,
                       QString *name,
                       QString *loggedInEmail,
                       QString *error = nullptr);

    // User-specific data
    bool loadUserData(QString email, QJsonObject *data,
                      QString *error = nullptr);

    bool saveUserData(QString email, const QJsonObject &data,
                      QString *error = nullptr);

private:
    QString filePath;

    bool saveAccounts(const QJsonObject &accounts);
    QString getUserDataPath(QString email);
};

#endif // AUTHMANAGER_H