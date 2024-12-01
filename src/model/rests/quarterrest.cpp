#include "quarterrest.h"

QuarterRest::QuarterRest(QWidget *parent)
    : QWidget{parent}
{
}

// Get the beat value of the rest
double QuarterRest::getBeatValue()
{
    return 1;
}
