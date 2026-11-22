#include "authmanager.h"
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QCryptographicHash>
#include <QStandardPaths>
#include <QUuid>

AuthManager::AuthManager()
{
    QString folder =
        QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation);

    QDir().mkpath(folder);

    filePath = folder + "/accounts.json";
}

// Save all registered accounts
bool AuthManager::saveAccounts(const QJsonObject &accounts)
{
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly))
        return false;

    file.write(QJsonDocument(accounts).toJson());
    file.close();

    return true;
}

// Create account
bool AuthManager::signup(QString name, QString email,
                         QString password, QString *error)
{
    name = name.trimmed();
    email = email.trimmed().toLower();

    if (name.isEmpty() || email.isEmpty() || password.isEmpty())
    {
        if (error)
            *error = "Please fill all fields.";

        return false;
    }

    if (password.length() < 6)
    {
        if (error)
            *error = "Password must be at least 6 characters.";

        return false;
    }

    QJsonObject accounts;
    QFile file(filePath);

    if (file.exists())
    {
        if (!file.open(QIODevice::ReadOnly))
        {
            if (error)
                *error = "Unable to read account file.";

            return false;
        }

        QJsonParseError parseError;

        QJsonDocument doc =
            QJsonDocument::fromJson(file.readAll(), &parseError);

        file.close();

        if (parseError.error != QJsonParseError::NoError
            || !doc.isObject())
        {
            if (error)
                *error = "Account file is invalid.";

            return false;
        }

        accounts = doc.object();
    }

    if (accounts.contains(email))
    {
        if (error)
            *error = "Email already registered.";

        return false;
    }

    QString salt = QUuid::createUuid().toString();

    QByteArray hash =
        QCryptographicHash::hash(
            (salt + password).toUtf8(),
            QCryptographicHash::Sha256);

    QJsonObject user;

    user["name"] = name;
    user["salt"] = salt;
    user["passwordHash"] =
        QString::fromLatin1(hash.toHex());

    accounts[email] = user;

    if (!saveAccounts(accounts))
    {
        if (error)
            *error = "Unable to save account.";

        return false;
    }

    return true;
}

// Existing login function
bool AuthManager::login(QString email, QString password,
                        QString *error)
{
    return loginWithUser(
        email, password, nullptr, nullptr, error);
}

// Login and return user details
bool AuthManager::loginWithUser(
    QString email,
    QString password,
    QString *name,
    QString *loggedInEmail,
    QString *error)
{
    email = email.trimmed().toLower();

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        if (error)
            *error = "Please create an account first.";

        return false;
    }

    QJsonParseError parseError;

    QJsonDocument doc =
        QJsonDocument::fromJson(file.readAll(), &parseError);

    file.close();

    if (parseError.error != QJsonParseError::NoError
        || !doc.isObject())
    {
        if (error)
            *error = "Account data is invalid.";

        return false;
    }

    QJsonObject accounts = doc.object();

    if (!accounts.contains(email))
    {
        if (error)
            *error = "Account not found. Please sign up.";

        return false;
    }

    QJsonObject user = accounts[email].toObject();

    QString salt = user["salt"].toString();

    QByteArray hash =
        QCryptographicHash::hash(
            (salt + password).toUtf8(),
            QCryptographicHash::Sha256);

    QString enteredHash =
        QString::fromLatin1(hash.toHex());

    if (enteredHash != user["passwordHash"].toString())
    {
        if (error)
            *error = "Incorrect password.";

        return false;
    }

    if (name)
        *name = user["name"].toString();

    if (loggedInEmail)
        *loggedInEmail = email;

    return true;
}

// Create a separate data file for each user
QString AuthManager::getUserDataPath(QString email)
{
    email = email.trimmed().toLower();

    QByteArray hash =
        QCryptographicHash::hash(
            email.toUtf8(),
            QCryptographicHash::Sha256);

    QString folder =
        QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation)
        + "/users";

    QDir().mkpath(folder);

    return folder + "/"
           + QString::fromLatin1(hash.toHex())
           + ".json";
}

// Load individual user's data
bool AuthManager::loadUserData(
    QString email,
    QJsonObject *data,
    QString *error)
{
    if (!data)
        return false;

    QString path = getUserDataPath(email);

    QFile file(path);

    // New user: start with empty data
    if (!file.exists())
    {
        *data = QJsonObject();
        return true;
    }

    if (!file.open(QIODevice::ReadOnly))
    {
        if (error)
            *error = "Unable to read user data.";

        return false;
    }

    QJsonParseError parseError;

    QJsonDocument doc =
        QJsonDocument::fromJson(file.readAll(), &parseError);

    file.close();

    if (parseError.error != QJsonParseError::NoError
        || !doc.isObject())
    {
        if (error)
            *error = "User data is invalid.";

        return false;
    }

    *data = doc.object();

    return true;
}

// Save individual user's data
bool AuthManager::saveUserData(
    QString email,
    const QJsonObject &data,
    QString *error)
{
    QString path = getUserDataPath(email);

    QFile file(path);

    if (!file.open(QIODevice::WriteOnly))
    {
        if (error)
            *error = "Unable to save user data.";

        return false;
    }

    file.write(QJsonDocument(data).toJson());
    file.close();

    return true;
}