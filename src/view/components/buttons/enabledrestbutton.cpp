#include "enabledrestbutton.h"

#define EMPTY_COLUMN "\u2015\n\u2015\n\u2015\n\u2015\n\u2015\n\u2015"

EnabledRestButton::EnabledRestButton(QWidget *parent)
    : QPushButton{parent}
{
    setText(EMPTY_COLUMN);
    setFixedSize(35, 205);
    setCheckable(true);
    setCursor(Qt::PointingHandCursor);
    setFocusPolicy(Qt::NoFocus);
    setStyleSheet
    (
        "QPushButton { "
        "   border-radius: 1px;"
        "   background-color: rgb(23,23,23);"
        "   font: 20pt Consolas;"
        "}"
        "QPushButton:hover { "
        "   background-color: rgb(75,75,75);"
        "}"
    );
}
