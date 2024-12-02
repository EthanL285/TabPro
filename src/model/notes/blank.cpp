#include "blank.h"

Blank::Blank()
    : Note{QVector<int>(6,-1)}
{
    setFixedWidth(35);
}

// Get the beat value of the note
double Blank::getBeatValue()
{
    return 0;
}
