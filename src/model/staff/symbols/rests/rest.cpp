#include "rest.h"

#include <QJsonObject>

// Getter for type
RestType Rest::getType()
{
    return type;
}

// Toggles selection of rest
void Rest::toggleSelect()
{
    if (isSelected)
        setStyleSheet(styleSheet().replace("color: rgb(70, 129, 232);", ""));
    else
        setStyleSheet(styleSheet() + " color: rgb(70, 129, 232);");

    isSelected = !isSelected;
}

// Converts the rest to JSON
QJsonObject Rest::toJson() const
{
    return QJsonObject{{"type", restToString(type)}};
}
