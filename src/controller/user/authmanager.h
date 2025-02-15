#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include "verificationstatus.h"
#include <QString>
#include <QtSql>
#include <QSslSocket>
#include <QObject>

struct UserCredentials
{
    QString email;
    QString password;
};

class AuthManager : public QObject
{
    Q_OBJECT

public:
    // Singleton instance
    static AuthManager& instance()
    {
        static AuthManager instance;
        return instance;
    }
    // Database functions
    void addUser(QString email, QString username, QString password);
    void rememberUser(const QString &email);
    bool tokenExists(const QString &email = QString());
    void removeToken();
    UserCredentials getUserCredentials();

    // Verification functions
    bool isRegisteredEmail(const QString &email);
    EmailStatus verifyEmail(const QString &email);
    UsernameStatus verifyUsername(const QString &username);
    PasswordStatus verifyPassword(const QString &password);
    UserStatus verifyUser(const QString &email, const QString &password);

    // Email verification functions
    QString getVerificationCode();
    void sendVerificationEmail(const QString &email);
    void updateUserPassword(const QString &email, const QString &password);
    void disconnectFromSMTPServer();

signals:
    void socketResponseReceived(const QString &response);

private slots:
    void onSocketResponse(const QString &response);
    void onSocketReadyRead(const QString &email);

private:
    QSqlDatabase userDatabase;
    QSslSocket *socket;
    QString verificationCode;

    QString hashPassword(const QString &password);
    QString generateVerificationCode();

    explicit AuthManager(QObject *parent = nullptr);
    ~AuthManager();
    AuthManager(const AuthManager&) = delete;
    AuthManager& operator=(const AuthManager&) = delete;
};

#endif // AUTHMANAGER_H
