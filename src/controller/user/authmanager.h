#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include "verificationstatus.h"

#include <QString>
#include <QtSql>
#include <QSslSocket>

struct UserCredentials
{
    QString email;
    QString password;
};

// Forward declarations
class PasswordUI;

// Handles all user information (destroy when user logs in)
class AuthManager: public QObject
{
    Q_OBJECT

public:
    explicit AuthManager(QWidget *parent = nullptr);

    // Database functions
    void addUser(QString email, QString username, QString password);
    void rememberUser(const QString &email);
    UserCredentials getUserCredentials();
    bool tokenExists(const QString &email = QString());
    void removeToken();
    QString hashPassword(const QString &password);

    // Verification functions
    bool isRegisteredEmail(const QString &email);
    EmailStatus verifyEmail(const QString &email);
    UsernameStatus verifyUsername(const QString &username);
    PasswordStatus verifyPassword(const QString &password);
    UserStatus verifyUser(const QString &email, const QString &password);

    // Email verification functions
    void sendVerificationEmail(const QString &userEmail, const QString &verificationCode, PasswordUI *passwordui);
    QString encodeBase64(const QByteArray &byteArray);
    void disconnectFromSMTPServer();
    void updateUserPassword(const QString &email, const QString &password);
    ~AuthManager();

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

#endif // AUTHMANAGER_H
