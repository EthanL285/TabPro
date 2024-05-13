#include "usermodel.h"

UserModel::UserModel() {
    userDatabase = QSqlDatabase::addDatabase("QSQLITE");
    userDatabase.setDatabaseName("users.db"); // Database stored in project file
}

void UserModel::addUser(QString email, QString username)
{
    if (userDatabase.open())
    {
        qDebug() << "Database is connected";
    }
    else
    {
        qDebug() << "Database is Not Connected";
    }

    userDatabase.transaction();

    QSqlQuery QueryInsertData(userDatabase);
    QueryInsertData.prepare("INSERT INTO registeredusers(Email, Username) VALUES(:email, :username)");
    QueryInsertData.bindValue(":email", email);
    QueryInsertData.bindValue(":username", username);
    QueryInsertData.exec();

    userDatabase.commit();
    userDatabase.close();
}
