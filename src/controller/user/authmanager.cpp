#include "authmanager.h"

#include <regex>
#include <QDnsLookup>
#include <QtNetworkAuth>
#include <QDesktopServices>
#include <QNetworkAccessManager>

AuthManager::AuthManager(QObject *parent)
    : QObject(parent)
{
    // Open database
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

// Returns the verification code
QString AuthManager::getVerificationCode()
{
    return verificationCode;
}

// Send verification code to user's email by connecting to Gmail SMTP servers
void AuthManager::sendVerificationEmail(const QString &email)
{
    if (socket == nullptr)
    {
        socket = new QSslSocket();
        connect(socket, &QSslSocket::readyRead, [=]() { AuthManager::onSocketReadyRead(email); });
        connect(socket, &QSslSocket::errorOccurred, [&]() { AuthManager::onSocketResponse(socket->errorString()); } );
    }
    verificationCode = generateVerificationCode();
    socket->connectToHostEncrypted("smtp.gmail.com", 465);
}

// Generates a random verification code
QString AuthManager::generateVerificationCode()
{
    QString code;
    for (int i = 0; i < 6; ++i)
    {
        code.append(QString::number(QRandomGenerator::global()->bounded(10)));
    }
    return code;
}

// Handles socket responses
void AuthManager::onSocketResponse(const QString &response)
{
    emit socketResponseReceived(response);
    disconnectFromSMTPServer();
}

// Reads SMTP responses from socket
void AuthManager::onSocketReadyRead(const QString &email)
{
    QString encodedEmail = QString(qgetenv("TabProEmail").toBase64());
    QString encodedPassword = QString(qgetenv("TabProKey").toBase64());
    QByteArray response = socket->readAll();

    // Handle 220: Server is ready for EHLO command (1)
    if (response.startsWith("220"))
    {
        socket->write("EHLO localhost\r\n");
        return;
    }
    // Handle 250: Server responses
    if (response.startsWith("250"))
    {
        // Server accepted EHLO command, proceed with authentication (2)
        if (response.contains("250-smtp.gmail.com at your service"))
        {
            socket->write("AUTH LOGIN\r\n");
            return;
        }
        // Server accepted MAIL FROM command, proceed with RCPT TO (6)
        if (response.contains("250 2.1.0 OK"))
        {
            socket->write("RCPT TO:<" + email.toUtf8() + ">\r\n");
            return;
        }
        // Send DATA command (7)
        if (response.contains("250 2.1.5 OK"))
        {
            socket->write("DATA\r\n");
            return;
        }
        // Server has accepted email data, proceed with QUIT (9)
        socket->write("QUIT\r\n");
        return;
    }
    // Handle 334: Server requests base64-encoded email or password
    if (response.startsWith("334"))
    {
        // Server requesting base64-encoded email (3)
        if (response.contains("VXNlcm5hbWU6"))
        {
            socket->write(encodedEmail.toUtf8() + "\r\n");
            return;
        }
        // Server requesting base64-encoded password (4)
        socket->write(encodedPassword.toUtf8() + "\r\n");
        return;
    }
    // Handle 235: Authentication successful, proceed with MAIL FROM (5)
    if (response.startsWith("235"))
    {
        socket->write("MAIL FROM:<tech.tabproapp@gmail.com>\r\n");
        return;
    }
    // Handle 354: Server ready to receive email data, send email (8)
    if (response.startsWith("354"))
    {
        QString emailData = "From: \"TabPro\" <tech.tabproapp@gmail.com>\r\n"
                            "To: <" + email + ">\r\n"
                            "Subject: Verification Code\r\n\r\n"
                            "Your verification code is: " + verificationCode + "\r\n.\r\n";
        socket->write(emailData.toUtf8());
        return;
    }
    // Handle 221: Close the connection (10)
    if (response.startsWith("221"))
    {
        onSocketResponse("Success");
        return;
    }
    // Handle unexpected responses (most likely environment variables are incorrect)
    onSocketResponse("\u2717 Error: Unexpected response from server");
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

// Disconnects socket from SMTP server
void AuthManager::disconnectFromSMTPServer()
{
    if (socket == nullptr || socket->state() == QAbstractSocket::UnconnectedState) return;
    socket->close();
    socket->deleteLater();
    socket = nullptr;
}

// Close the database
AuthManager::~AuthManager()
{
    userDatabase.close();
}
