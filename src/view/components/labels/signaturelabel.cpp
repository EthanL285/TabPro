#include "signaturelabel.h"

SignatureLabel::SignatureLabel(QString text, QWidget *parent)
    : QLabel{parent}
{
    setText(text);
    setFixedSize(30, 30);
    setFont(QFont("Bravura Text", 25));
    setAlignment(Qt::AlignCenter);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet
    (
        "QLabel {"
        "   border: none;"
        "}"
        "QLabel:hover {"
        "   color: rgb(70, 129, 232);"
        "}"
    );
}
