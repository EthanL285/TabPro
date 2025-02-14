#include "authmanager.h"
#include "passwordui.h"

#include <regex>
#include <QDnsLookup>
#include <QtNetworkAuth>
#include <QDesktopServices>
#include <QNetworkAccessManager>

AuthManager::AuthManager(QWidget *parent)
    : QObject{parent}
{
    userDatabase = QSqlDatabase::addDatabase("QSQLITE");
    userDatabase.setDatabaseName("users.db");
    userDatabase.open();
}

//////////////////////////////////////////////////////////////////////
//                       DATABASE FUNCTIONS                         //
//////////////////////////////////////////////////////////////////////

// Adds user to database
void AuthManager::addUser(QString email, QString username, QString password)
{
    QString hashedPassword = hashPassword(password);
    userDatabase.transaction();

    QSqlQuery QueryInsertData(userDatabase);
    QueryInsertData.prepare("INSERT INTO users(Email, Username, Password) VALUES(:email, :username, :password)");
    QueryInsertData.bindValue(":email", email.toLower());
    QueryInsertData.bindValue(":username", username);
    QueryInsertData.bindValue(":password", hashedPassword);
    QueryInsertData.exec();
    userDatabase.commit();
}

// Remembers user credentials
void AuthManager::rememberUser(const QString &email)
{
    QSqlQuery query;
    userDatabase.transaction();

    // Reset tokens for all users
    query.prepare("UPDATE users SET Remember = 0");
    query.exec();

    // Set token for user
    query.prepare("UPDATE users SET Remember = 1 WHERE Email = :email");
    query.bindValue(":email", email);
    query.exec();

    userDatabase.commit();
}

// Get credentials of user with existing token
UserCredentials AuthManager::getUserCredentials()
{
    QSqlQuery query;
    query.prepare("SELECT Email, Password FROM users WHERE Remember = 1");
    query.exec();

    // Token found
    UserCredentials usercredentials;
    if (query.next())
    {
        usercredentials.email = query.value(0).toString();
        usercredentials.password = query.value(1).toString();
    }
    return usercredentials;
}

// Checks for existing tokens
bool AuthManager::tokenExists(const QString &email)
{
    QSqlQuery query;
    QString queryString = "SELECT 1 FROM users WHERE Remember = 1";

    // Check tokens for all users
    if (!email.isEmpty())
    {
        queryString += " AND Email = :email";
        query.prepare(queryString);
        query.bindValue(":email", email);
    }
    // Check tokens for a specific user
    else
    {
        query.prepare(queryString);
    }
    return query.exec() && query.next();
}

void AuthManager::removeToken()
{
    // Remove token for all users
    QSqlQuery query;
    query.prepare("UPDATE users SET Remember = 0");
    query.exec();
}

// Updates the users passwords
void AuthManager::updateUserPassword(const QString &email, const QString &password)
{
    QSqlQuery query;
    QString hashedPassword = hashPassword(password);
    query.prepare("UPDATE users SET Password = :password WHERE Email = :email");
    query.bindValue(":email", email);
    query.bindValue(":password", hashedPassword);
    query.exec();
}

// Hashes the user's password
QString AuthManager::hashPassword(const QString &password)
{
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

//////////////////////////////////////////////////////////////////////
//                     VERIFICATION FUNCTIONS                       //
//////////////////////////////////////////////////////////////////////

// Verifies the user's email and password, and returns the result
UserStatus AuthManager::verifyUser(const QString &email, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT Password FROM users WHERE LOWER(Email) = LOWER(:email)");
    query.bindValue(":email", email.toLower());

    if (!query.exec() || !query.next())
        return UserStatus::EmailNotFound;

    return (query.value(0).toString() == hashPassword(password))
        ? UserStatus::Valid
        : UserStatus::IncorrectPassword;
}

// Checks if the given email is registered
bool AuthManager::isRegisteredEmail(const QString &email)
{
    QSqlQuery query;
    query.prepare("SELECT 1 FROM users WHERE LOWER(Email) = LOWER(:email) LIMIT 1");
    query.bindValue(":email", email.toLower());

    return query.exec() && query.next();
}

/* Email Validation Requirements:
     *
     * Local Part Restrictions:
     *    1. Can contain letters, digits, dots, hypens and underscores
     *    2. Cannot start or end with a dot
     *    3. Consecutive dots are not allowed
     *
     * Domain Part Restrictions:
     *    1. Can contain letters, digits, dots and hypens
     *    2. TLD must be at least two characters long and consist of letters only
     *    3. Domain part of email has valid MX (Mail Exchange) records.
     *
     * Length Limits:
     *    1. Should not exceed 255 characters
*/

// Verifies the given email
EmailStatus AuthManager::verifyEmail(const QString &email)
{
    if (email.length() > 255)
        return EmailStatus::LimitExceeded;

    std::regex pattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!regex_match(email.toStdString(), pattern))
        return EmailStatus::InvalidFormat;

    if (isRegisteredEmail(email))
        return EmailStatus::EmailInUse;

    // Peform DNS lookup for MX records
    QString domain = email.mid(email.indexOf("@") + 1);
    QDnsLookup dnsLookup = QDnsLookup();
    dnsLookup.setType(QDnsLookup::MX);
    dnsLookup.setName(domain);

    QEventLoop loop;
    QObject::connect(&dnsLookup, &QDnsLookup::finished, &loop, &QEventLoop::quit);
    dnsLookup.lookup();
    loop.exec();

    // Check if MX records exist
    if (dnsLookup.error() == QDnsLookup::NoError && !dnsLookup.mailExchangeRecords().isEmpty())
        return EmailStatus::Valid;

    return EmailStatus::InvalidDomain;
}

// Verifies the given username
UsernameStatus AuthManager::verifyUsername(const QString &username)
{
    return (username.length() > 20) ? UsernameStatus::LimitExceeded : UsernameStatus::Valid;
}

// Verifies the given password
PasswordStatus AuthManager::verifyPassword(const QString &password)
{
    return (password.length() < 8) ? PasswordStatus::InsufficientLength : PasswordStatus::Valid;
}

//////////////////////////////////////////////////////////////////////
//                  EMAIL VERIFICATION FUNCTIONS                    //
//////////////////////////////////////////////////////////////////////

// Send verrification code to user's email by connecting to Gmail SMTP servers
void AuthManager::sendVerificationEmail(const QString &userEmail, const QString &verificationCode, PasswordUI *passwordui)
{
    this->passwordui = passwordui;
    this->userEmail = userEmail;
    this->verificationCode = verificationCode;

    // Initialize a secure TCP socket for connecting to the SMTP server
    if (socket == nullptr) {
        socket = new QSslSocket(this);

        // Connect socket signal to slot to avoid blocking program
        connect(socket, &QSslSocket::readyRead, this, &AuthManager::socketReadyRead); // Read response from server
        connect(socket, &QSslSocket::errorOccurred, this, &AuthManager::socketError); // Socket error
    }

    // Connect to the Gmail SMTP server if socket is inactive
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        passwordui->disableEmailField(true);
        passwordui->addErrorMessage("⏳ Connecting to SMTP server...", 152, "gray");
        socket->connectToHostEncrypted("smtp.gmail.com", 465);
    }
}

// Converts a QString to Base64
QString AuthManager::encodeBase64(const QByteArray &byteArray)
{
    return QString(byteArray.toBase64());
}

// Handles socket errors
void AuthManager::socketError(QAbstractSocket::SocketError error)
{
    // Print error message
    passwordui->disableEmailField(false);
    passwordui->addErrorMessage(QString::fromUtf8("\u2717 ") + "Error: " + socket->errorString(), 153, "rgb(237, 67, 55)");
    disconnectFromSMTPServer();
}

// Reads SMTP responses from socket and handles different responses accordingly
void AuthManager::socketReadyRead()
{
    QString encodedEmail = encodeBase64(qgetenv("TabProEmail"));
    QString encodedPassword = encodeBase64(qgetenv("TabProKey"));

    // Read data from the socket
    QByteArray response = socket->readAll();
    qDebug() << response;

    // Handle different SMTP responses (numbers indicate the calling order)
    if (response.startsWith("220"))
    {
        // Server is ready for EHLO command  (1)
        socket->write("EHLO localhost\r\n");
    }
    else if (response.startsWith("250"))
    {
        // Server has accepted EHLO command, proceed with authentication (2)
        if (response.startsWith("250-smtp.gmail.com at your service"))
        {
            socket->write("AUTH LOGIN\r\n");
        }
        // Server has accepted MAIL FROM command, proceed with RCPT TO (6)
        else if (response.startsWith("250 2.1.0 OK"))
        {
            socket->write("RCPT TO:<" + userEmail.toUtf8() + ">\r\n");
        }
        // Send DATA command (7)
        else if (response.startsWith("250 2.1.5 OK"))
        {
            socket->write("DATA\r\n");
        }
        // Server has accepted email data, proceed with QUIT (9)
        else
        {
            socket->write("QUIT\r\n");
        }
    }
    else if (response.startsWith("334"))
    {
        // Server is requesting base64-encoded email (3)
        if (response.startsWith("334 VXNlcm5hbWU6"))
        {
            socket->write(encodedEmail.toUtf8() + "\r\n");
        }
        // Server is requesting base64-encoded email (4)
        else
        {
            socket->write(encodedPassword.toUtf8() + "\r\n");
        }
    }
    else if (response.startsWith("235"))
    {
        // Authentication successful, proceed with MAILFROM (5)
        socket->write("MAIL FROM:<tech.tabproapp@gmail.com>\r\n");
    }
    else if (response.startsWith("354"))
    {
        // Server is ready to receive email data, send the email (8)
        QString emailHeaders = "From: \"TabPro\" <tech.tabproapp@gmail.com>\r\n";
        emailHeaders += "To: <" + userEmail + ">\r\n";

        socket->write(emailHeaders.toUtf8());
        socket->write("Subject: Verification Code\r\n");
        socket->write("\r\n");
        socket->write("Your verification code is: " + verificationCode.toUtf8() + "\r\n");
        socket->write(".\r\n");
    }
    else if (response.startsWith("221"))
    {
        // Close the connection (10)
        disconnectFromSMTPServer();

        // Email Successful, remove connecting to SMTP server message
        passwordui->removeErrorMessage(0, 172);  // Error message doesn't delete before onEmailSentSuccess() since there is timer in removeErrorMessage()

        // Add timer to transition once error message is removed

        QTimer *timer = new QTimer(this);
        timer->setSingleShot(true);

        connect(timer, &QTimer::timeout, this, [this, timer]()
        {
            passwordui->disableEmailField(false);
            passwordui->onEmailSentSuccess();
            delete timer;
        });
        timer->start(0);
    }
    else
    {
        // Unexpected response from server, print error message (most likely environment variables are incorrect)
        passwordui->disableEmailField(false);
        passwordui->addErrorMessage(QString::fromUtf8("\u2717 ") + "Error: Unexpected response from server", 153, "rgb(237, 67, 55)");
        disconnectFromSMTPServer();
    }
}

/*
 * For Reference, below is the log of a successful SMTP communication session with Gmail's SMTP server:
 *
 * (1) "220 smtp.gmail.com ESMTP 41be03b00d2f7-6340a632725sm17786173a12.12 - gsmtp\r\n"
 * (2) "250-smtp.gmail.com at your service, [61.68.176.27]\r\n250-SIZE 35882577\r\n250-8BITMIME\r\n250-AUTH LOGIN PLAIN XOAUTH2 PLAIN-CLIENTTOKEN OAUTHBEARER XOAUTH\r\n250-ENHANCEDSTATUSCODES\r\n250-PIPELINING\r\n250-CHUNKING\r\n250 SMTPUTF8\r\n"
 * (3) "334 VXNlcm5hbWU6\r\n"
 * (4) "334 UGFzc3dvcmQ6\r\n"
 * (5) "235 2.7.0 Accepted\r\n"
 * (6) "250 2.1.0 OK 41be03b00d2f7-6340a632725sm17786173a12.12 - gsmtp\r\n"
 * (7) "250 2.1.5 OK 41be03b00d2f7-6340a632725sm17786173a12.12 - gsmtp\r\n"
 * (8) "354  Go ahead 41be03b00d2f7-6340a632725sm17786173a12.12 - gsmtp\r\n"
 * (9) "250 2.0.0 OK  1716281641 41be03b00d2f7-6340a632725sm17786173a12.12 - gsmtp\r\n"
 * (10) "221 2.0.0 closing connection 41be03b00d2f7-6340a632725sm17786173a12.12 - gsmtp\r\n"
 */

// Disconnects user from SMTP server
void AuthManager::disconnectFromSMTPServer()
{
    if (socket == nullptr || socket->state() == QAbstractSocket::UnconnectedState) return;
    socket->close();
    socket->deleteLater();
    socket = nullptr;
}

// Destructor
AuthManager::~AuthManager()
{
    userDatabase.close();
}
