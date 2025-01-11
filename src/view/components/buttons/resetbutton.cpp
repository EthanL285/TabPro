#include "resetbutton.h"

ResetButton::ResetButton(QSize size, QString tooltip, QWidget *parent)
    : QPushButton{parent}
{
    setFixedSize(size);
    setCursor(Qt::PointingHandCursor);
    setToolTip(tooltip);
    setStyleSheet
    (
        "QPushButton {"
        "   image: url(:/miscellaneous/miscellaneous/trash.png);"
        "   background-color: transparent;"
        "   border: none;"
        "   padding: 1px 1px;"
        "}"
        "QPushButton:hover {"
        "   image: url(:/miscellaneous/miscellaneous/trash hover.png)"
        "}"
    );
}

