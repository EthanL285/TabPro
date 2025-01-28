#include "rest.h"

// Toggles selection of rest
void Rest::toggleSelect()
{
    if (isSelected)
        setStyleSheet(styleSheet().replace("color: rgb(70, 129, 232);", ""));
    else
        setStyleSheet(styleSheet() + " color: rgb(70, 129, 232);");

    isSelected = !isSelected;
}
