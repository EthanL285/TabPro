#include "disabledrestbutton.h"

#define EMPTY_COLUMN "\u2015\n\u2015\n\u2015\n\u2015\n\u2015\n\u2015"

DisabledRestButton::DisabledRestButton(QWidget *parent)
    : QPushButton{parent}
{
    setText(EMPTY_COLUMN);
    setFixedSize(35, 205);
    setFocusPolicy(Qt::NoFocus);
    setStyleSheet
    (
        "border-radius: 1px;"
        "background-color: rgb(23,23,23);"
        "font: 20pt Consolas;"
    );

}
