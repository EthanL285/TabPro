#ifndef USERMODEL_H
#define USERMODEL_H

#include <QString>
#include <QtSql>
#include "uiwidgets.h"

struct UserCredentials
{
    QString email;
    QString password;
};

class UserModel // Destroy class when user logs in
{
public:
    UserModel();
    void addUser(QString email, QString username, QString password);
    QString isValidEmail(const QString &email);
    QString isValidPassword(const QString &password);
    QString isValidUsername(const QString &username);
    bool isUniqueEmail(const QString &email);
    QString hashPassword(const QString &password);
    QString verifyUser(TextField *emailParent, TextField *passwordParent);
    void rememberUserCredentials(const QString &email);
    bool tokenExists(const QString &email = QString());
    void removeToken();
    UserCredentials getUserCredentials();

private:
    QSqlDatabase userDatabase;
};

#endif // USERMODEL_H
