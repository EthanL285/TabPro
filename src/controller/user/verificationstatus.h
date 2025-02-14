#ifndef VERIFICATIONSTATUS_H
#define VERIFICATIONSTATUS_H

#include <QString>
#include <QMap>

enum class UserStatus
{
    Valid,
    EmailNotFound,
    IncorrectPassword
};

enum class EmailStatus
{
    Valid,
    InvalidFormat,
    InvalidDomain,
    LimitExceeded,
    EmailInUse
};

enum class UsernameStatus
{
    Valid,
    LimitExceeded
};

enum class PasswordStatus
{
    Valid,
    InsufficientLength
};

const QMap<UserStatus, QString> userStatusMap =
{
    {UserStatus::Valid, "Valid"},
    {UserStatus::EmailNotFound, "\u2717 Email not found. Please check email address"},
    {UserStatus::IncorrectPassword, "\u2717 Incorrect password. Please check password"}
};

const QMap<EmailStatus, QString> emailStatusMap =
{
    {EmailStatus::Valid, "Valid"},
    {EmailStatus::InvalidFormat, "\u2717 Please enter a valid email address"},
    {EmailStatus::InvalidDomain, "\u2717 Invalid domain. Please check email address"},
    {EmailStatus::LimitExceeded, "\u2717 Email address exceeds limit (255 characters)"},
    {EmailStatus::EmailInUse, "\u2717 Email address already in use"}
};

const QMap<UsernameStatus, QString> usernameStatusMap =
{
    {UsernameStatus::Valid, "Valid"},
    {UsernameStatus::LimitExceeded, "\u2717 Username must not exceed 20 characters"}
};

const QMap<PasswordStatus, QString> passwordStatusMap =
{
    {PasswordStatus::Valid, "Valid"},
    {PasswordStatus::InsufficientLength, "\u2717 Password must be 8 characters minimum"}
};

#endif // VERIFICATIONSTATUS_H
