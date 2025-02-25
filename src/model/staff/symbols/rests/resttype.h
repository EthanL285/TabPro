#ifndef RESTTYPE_H
#define RESTTYPE_H

#include <QString>

enum class RestType
{
    Eighth,
    Half,
    Quarter
};

inline const QString restToString(RestType type)
{
    switch (type)
    {
        case RestType::Eighth: return "Eighth";
        case RestType::Half: return "Half";
        case RestType::Quarter: return "Quarter";
        default: qFatal("Invalid RestType provided");
    }
}

#endif // RESTTYPE_H
