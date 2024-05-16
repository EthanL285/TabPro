#include "usermodel.h"
#include <regex>
#include <QDnsLookup>

UserModel::UserModel() {
    userDatabase = QSqlDatabase::addDatabase("QSQLITE");
    userDatabase.setDatabaseName("users.db"); // Database stored in project file
}

// Add user to database
void UserModel::addUser(QString email, QString username)
{
    userDatabase.open();
    userDatabase.transaction();

    QSqlQuery QueryInsertData(userDatabase);
    QueryInsertData.prepare("INSERT INTO registeredusers(Email, Username) VALUES(:email, :username)");
    QueryInsertData.bindValue(":email", email);
    QueryInsertData.bindValue(":username", username);
    QueryInsertData.exec();

    userDatabase.commit();
    userDatabase.close();
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
