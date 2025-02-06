#include "signaturehelper.h"

static const int BASE = 0xE080;

SignatureHelper::SignatureHelper()
{
}

// Converts the given unicode to a number
int SignatureHelper::unicodeToNum(QString unicode)
{
    // Single digit number
    if (unicode.length() == 1)
        return unicode.at(0).unicode() - BASE;

    // Double digit number
    int firstDigit = unicode.at(0).unicode() - BASE;
    int secondDigit = unicode.at(unicode.length() - 1).unicode() - BASE;

    return firstDigit * 10 + secondDigit;
}

// Converts the given number to unicode
QString SignatureHelper::numToUnicode(int num)
{
    // Single digit number
    if (num < 10)
        return QString(QChar(BASE + num));

    // Double digit number
    int firstDigit = num / 10;
    int secondDigit = num % 10;
    QString space = (firstDigit == 1) ? "- " : "--";

    return QString(QChar(BASE + firstDigit)) + space + QChar(BASE + secondDigit);
}

// Parses the unicode time signature
QPair<int, int> SignatureHelper::parseSignature(QString signature)
{
    int beatsPerMeasure = signature.at(1).unicode() - BASE;
    int beatUnit = signature.at(3).unicode() - BASE;

    return QPair(beatsPerMeasure, beatUnit);
}

