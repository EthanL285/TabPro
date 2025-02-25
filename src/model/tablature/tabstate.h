#ifndef TABSTATE_H
#define TABSTATE_H

#include <QColor>
#include <QHash>

enum class State
{
    Normal,
    Hovered,
    Selected
};

const QHash<State, QColor> columnStateColours =
{
    { State::Normal, QColor(23, 23, 23) },
    { State::Hovered, QColor(50, 50, 50) },
    { State::Selected, QColor(40, 40, 40) }
};

const QHash<State, QColor> fretStateColours =
{
    { State::Normal, Qt::transparent },
    { State::Hovered, QColor(75, 75, 75) },
    { State::Selected, QColor(70, 129, 232) }
};

#endif // TABSTATE_H
