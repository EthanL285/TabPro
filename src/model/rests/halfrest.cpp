#include "halfrest.h"

HalfRest::HalfRest(QWidget *parent)
    : QWidget{parent}
{
}

// Get the beat value of the rest
double HalfRest::getBeatValue()
{
    return 2;
}
