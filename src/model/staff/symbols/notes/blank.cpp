#include "blank.h"
#include "tablature.h"

Blank::Blank()
    : Note{QVector<int>(6,-1)}
{
    setFixedWidth(Tablature::DEFAULT_COLUMN_WIDTH);
}

// Get the beat value of the note
double Blank::getBeatValue()
{
    return 0;
}
