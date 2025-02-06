#ifndef SIGNATUREHELPER_H
#define SIGNATUREHELPER_H

#include <QString>

class SignatureHelper
{
public:
    SignatureHelper();
    static int unicodeToNum(QString unicode);
    static QString numToUnicode(int num);
    static QPair<int, int> parseSignature(QString signature);
};

#endif // SIGNATUREHELPER_H
