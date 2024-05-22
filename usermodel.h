#ifndef USERMODEL_H
#define USERMODEL_H

#include <QString>
#include <QtSql>
#include <QSslSocket>
#include "uiwidgets.h"

struct UserCredentials
{
    QString email;
    QString password;
};

// Forward declarations
class PasswordUI;

// Handles all user information (destroy when user logs in)
class UserModel : public QObject
{
    Q_OBJECT

public:
    UserModel();
    void addUser(QString email, QString username, QString password);
    QString isValidEmail(const QString &email);
    QString isValidPassword(const QString &password);
    QString isValidUsername(const QString &username);
    bool isUniqueEmail(const QString &email);
    QString hashPassword(const QString &password);
    QString verifyUser(TextField *emailParent, TextField *passwordParent = nullptr);
    void rememberUserCredentials(const QString &email);
    bool tokenExists(const QString &email = QString());
    void removeToken();
    UserCredentials getUserCredentials();
    void sendVerificationEmail(const QString &userEmail, const QString &verificationCode, PasswordUI *passwordui);
    QString encodeBase64(const QByteArray &byteArray);

protected slots:
    void socketError(QAbstractSocket::SocketError error);
    void socketReadyRead();

private:
    QSqlDatabase userDatabase;
    QByteArray emailData;
    QSslSocket *socket = nullptr;
    QString userEmail;
    QString verificationCode;
    PasswordUI *passwordui = nullptr;
};

#endif // USERMODEL_H
