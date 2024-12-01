#include "blank.h"

Blank::Blank(QWidget *parent)
    : Note{QVector<int>(6,-1), parent}
{
    setFixedWidth(35);
}

// Get the beat value of the note
double Blank::getBeatValue()
{
    return 0;
}
