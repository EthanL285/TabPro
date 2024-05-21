#include "usermodel.h"
#include <regex>
#include <QDnsLookup>
#include <QtNetworkAuth>
#include <QDesktopServices>
#include <QNetworkAccessManager>

UserModel::UserModel()
{
    userDatabase = QSqlDatabase::addDatabase("QSQLITE");
    userDatabase.setDatabaseName("users.db"); // Database stored in project file
}

// Add user to database
void UserModel::addUser(QString email, QString username, QString password)
{
    // Hash password
    QString hashedPassword = hashPassword(password);

    // Add user
    userDatabase.open();
    userDatabase.transaction();

    QSqlQuery QueryInsertData(userDatabase);
    QueryInsertData.prepare("INSERT INTO registeredusers(Email, Username, Password) VALUES(:email, :username, :password)");
    QueryInsertData.bindValue(":email", email.toLower());
    QueryInsertData.bindValue(":username", username);
    QueryInsertData.bindValue(":password", hashedPassword);
    QueryInsertData.exec();

    userDatabase.commit();
    userDatabase.close();
}

// Remember user's credentials for future sessions
void UserModel::rememberUserCredentials(const QString &email)
{
    // Check if database connection is already open
    if (!userDatabase.isOpen())
    {
        userDatabase.open();
    }
    QSqlQuery query;
    userDatabase.transaction();

    // Reset Remember value for all users
    query.prepare("UPDATE registeredusers SET Remember = 0");
    query.exec();

    // Set user's Remember value in database to 1 (true)
    query.prepare("UPDATE registeredusers SET Remember = 1 WHERE Email = :email");
    query.bindValue(":email", email);
    query.exec();

    userDatabase.commit();
}

// Checks for any existing tokens
bool UserModel::tokenExists(const QString &email) // Email is an optional parameter (assigned in declaration)
{
    // Check if database connection is already open
    if (!userDatabase.isOpen())
    {
        userDatabase.open();
    }
    QSqlQuery query;

    // Check for tokens in database for all users
    if (email.isEmpty())
    {
        query.prepare("SELECT Remember FROM registeredusers WHERE Remember = 1");
    }
    // Check for tokens in database for specific user
    else
    {
        query.prepare("SELECT Remember FROM registeredusers WHERE Remember = 1 AND Email = :email");
        query.bindValue(":email", email);
    }

    // Return value
    query.exec();
    if (query.next())
    {
        return true;
    }
    return false;
}

void UserModel::removeToken()
{
    // Check if database connection is already open
    if (!userDatabase.isOpen())
    {
        userDatabase.open();
    }

    // Remove token for all users
    QSqlQuery query;
    query.prepare("UPDATE registeredusers SET Remember = 0");
    query.exec();
}

// Get credentials of user with existing token
UserCredentials UserModel::getUserCredentials()
{
    UserCredentials usercredentials;

    // Check if database connection is already open
    if (!userDatabase.isOpen())
    {
        userDatabase.open();
    }
    // Get credentials of user with existing token
    QSqlQuery query;
    query.prepare("SELECT Email, Password FROM registeredusers WHERE Remember = 1");
    query.exec();

    // Token found
    if (query.next())
    {
        usercredentials.email = query.value(0).toString();
        usercredentials.password = query.value(1).toString();
    }
    // No token found
    else
    {
        // Do nothing :)
    }

    return usercredentials;
}

// Hashes user's password
QString UserModel::hashPassword(const QString &password)
{
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256); // Compute hash of password
    return QString(hash.toHex()); // Convert binary hash value to a hexadecimal string (Human-readable representaiton)
}

// Retrieve user's hashed password
QString UserModel::verifyUser(TextField *emailParent, TextField *passwordParent) // Optional password parameter
{
    // Retrieve fields from parent widgets
    QLineEdit *email = emailParent->findChild<QLineEdit*>("field");
    QString message;

    // Check if database connection is already open
    if (!userDatabase.isOpen())
    {
        userDatabase.open();
    }

    // Retrieve hashed password from database
    QSqlQuery query;
    query.prepare("SELECT Password FROM registeredusers WHERE LOWER(Email) = LOWER(:email)");
    query.bindValue(":email", email->text().toLower());
    query.exec();

    // Email is registered in database
    if (query.next())
    {
        // Password parameter is provided
        if (passwordParent != nullptr)
        {
            QLineEdit *password = passwordParent->findChild<QLineEdit*>("field");
            QString storedHashedPassword = query.value(0).toString(); // Retrieve hashed password from first column (0)

            // Compare hashed passwords
            if (storedHashedPassword == hashPassword(password->text()))
            {
                message = "Valid";
            }
            else
            {
                message = QString::fromUtf8("\u2717 ") + "Incorrect password. Please check password";
                passwordParent->setRedBorder(true);
                password->setFocus();
            }
        }
        // Password parameter is not provided
        else
        {
            message = "Valid";
        }
    }
    // Email is not registered in database
    else
    {
        message = QString::fromUtf8("\u2717 ") + "Email not found. Please check email address";
        emailParent->setRedBorder(true);
        email->setFocus();
    }
    return message;
}

// Checks if email is unique
bool UserModel::isUniqueEmail(const QString &email)
{
    bool isUnique = false;

    // Check if database connection is already open
    if (!userDatabase.isOpen())
    {
        userDatabase.open();
    }

    QSqlQuery query;
    query.prepare("SELECT 1 FROM registeredusers WHERE LOWER(Email) = LOWER(:email) LIMIT 1");
    query.bindValue(":email", email.toLower());
    query.exec();

    // Check if there is at least one result that matches
    isUnique = !query.next();

    return isUnique;
}

// Checks if email is valid
QString UserModel::isValidEmail(const QString &email)
{
    std::string stdEmail = email.toStdString();
    QString message;

    // Email exceeds character limit
    if (email.length() > 255)
    {
        message = QString::fromUtf8("\u2717 ") + "Email address exceeds limit (255 characters)";
        return message;
    }

    // Compare email with regular expression pattern
    std::regex pattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    bool match = regex_match(stdEmail, pattern);

    // Invalid email format
    if (!match)
    {
        message = QString::fromUtf8("\u2717 ") + "Please enter a valid email address";
        return message;
    }

    // Email is not unique
    if (!isUniqueEmail(email))
    {
        message = QString::fromUtf8("\u2717 ") + "Email address already in use";
        return message;
    }

    // Retrieve domain name
    auto it = stdEmail.find("@");
    std::string stdDomain = stdEmail.substr(it + 1);
    QString domain = QString::fromStdString(stdDomain);

    // Peform DNS lookup for MX records
    QDnsLookup *dnsLookup = new QDnsLookup();
    dnsLookup->setType(QDnsLookup::MX);
    dnsLookup->setName(domain);

    // Connect to the finished signal to handle lookup results
    QEventLoop loop;
    QObject::connect(dnsLookup, &QDnsLookup::finished, &loop, &QEventLoop::quit); // Quit loop once lookup is finished
    dnsLookup->lookup();
    loop.exec();

    bool isValid = false;
    if (dnsLookup->error() == QDnsLookup::NoError && !dnsLookup->mailExchangeRecords().isEmpty())
    {
        isValid = true;
    }

    delete dnsLookup;

    if (isValid)
    {
        return "Valid";
    }
    else
    {
        message = QString::fromUtf8("\u2717 ") + "Invalid domain. Please check email address";
        return message; // Invalid domain
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
}

// Checks if password is valid
QString UserModel::isValidPassword(const QString &password)
{
    // Password should be at least 8 characters long
    if (password.length() < 8)
    {
        QString message = QString::fromUtf8("\u2717 ") + "Password must be 8 characters minimum";
        return message;
    }
    return "Valid";
}

// Checks if username is valid
QString UserModel::isValidUsername(const QString &username)
{
    // Username must not exceed 20 characters
    if (username.length() > 20)
    {
        QString message = QString::fromUtf8("\u2717 ") + "Username must not exceed 20 characters";
        return message;
    }
    return "Valid";
}

// Send verrification code to user's email by connecting to Gmail SMTP servers
void UserModel::sendVerificationEmail(const QString &userEmail, const QString &verificationCode)
{
    // Uncomment this ----------------------------------------------------------------
    this->userEmail = userEmail;
    this->verificationCode = verificationCode;

    // Initialize a secure TCP socket for connecting to the SMTP server
    if (socket == nullptr) {
        socket = new QSslSocket(this);

        // Connect socket signal to slot to avoid blocking program
        connect(socket, &QSslSocket::readyRead, this, &UserModel::socketReadyRead); // Read response from server
        connect(socket, &QSslSocket::errorOccurred, this, &UserModel::socketError); // Socket error
    }

    // Connect to the Gmail SMTP server
    socket->connectToHostEncrypted("smtp.gmail.com", 465);
}

// Converts a QString to Base64
QString UserModel::encodeBase64(const QByteArray &byteArray)
{
    return QString(byteArray.toBase64());
}

// Handles socket errors
void UserModel::socketError(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error:" << socket->errorString(); // "Host not found" if no internet
    socket->close();
}

// Reads SMTP responses from socket and handles different responses accordingly
void UserModel::socketReadyRead()
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
        socket->close();
        delete socket;
        socket = nullptr;
    }
    else
    {
        // Unexpected response from server, print error message
        qDebug() << "Error: Unexpected response from server: " << response;
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

