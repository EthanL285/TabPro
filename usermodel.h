#ifndef USERMODEL_H
#define USERMODEL_H

#include <QString>
#include <QtSql>

class UserModel // Destroy class when user logs in
{
public:
    UserModel();
    void addUser(QString email, QString username);
    QString isValidEmail(const QString &email);
    QString isValidPassword(const QString &password);
    QString isValidUsername(const QString &username);

private:
    QSqlDatabase userDatabase;
};

#endif // USERMODEL_H
