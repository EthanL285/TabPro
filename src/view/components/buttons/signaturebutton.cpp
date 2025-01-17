#include "signaturebutton.h"

#define COMMON_TIME "\uE09E\uE084\uE09F\uE084"

SignatureButton::SignatureButton(QWidget *parent)
    : QPushButton{parent}
{
    setText(COMMON_TIME);
    setFont(QFont("Bravura Text", 20));
    setFixedSize(40,23);
    setCursor(Qt::PointingHandCursor);
    setToolTip("Time Signature");
    setObjectName("time signature");
    setStyleSheet
    (
        "QPushButton {"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   color: rgb(70, 129, 232)"
        "}"
    );
}

