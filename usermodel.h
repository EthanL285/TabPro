#ifndef USERMODEL_H
#define USERMODEL_H

#include <QString>
#include <QtSql>

class UserModel // Destroy class when user logs in
{
public:
    UserModel();
    void addUser(QString email, QString username);
    bool validateEmail(const QString &email);

private:
    QSqlDatabase userDatabase;
};

#endif // USERMODEL_H
