#ifndef USERMODEL_H
#define USERMODEL_H

#include <QString>
#include <QtSql>

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
    QString verifyUser(const QString &email, const QString &password);

private:
    QSqlDatabase userDatabase;
};

#endif // USERMODEL_H
