#include "signaturebutton.h"

SignatureButton::SignatureButton(QString text, QSize size, int fontSize, QString toolTip, QWidget *parent)
    : QPushButton{parent}
{
    setText(text);
    setFont(QFont("Bravura Text", fontSize));
    setFixedSize(size);
    setCursor(Qt::PointingHandCursor);
    setToolTip(toolTip);
    setObjectName(toolTip);
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

