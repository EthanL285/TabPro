#include "signaturebutton.h"

SignatureButton::SignatureButton(QWidget *parent)
    : QPushButton{parent}
{
    setFixedSize(40,23);
    setCursor(Qt::PointingHandCursor);
    setToolTip("Time Signature");
    setObjectName("time signature");
    setStyleSheet
    (
        "QPushButton {"
        "   image: url(:/menubar/menubar/Time Signature.png);"
        "   background-color: transparent;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   image: url(:/menubar/menubar/Time Signature Hover.png)"
        "}"
    );
}

