#include "wholerest.h"

WholeRest::WholeRest(QWidget *parent)
    : QWidget{parent}
{}

// Get the beat value of the rest
double WholeRest::getBeatValue()
{
    return 4;
}
