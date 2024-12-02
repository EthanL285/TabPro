#include "halfrest.h"

HalfRest::HalfRest()
{
    setFixedWidth(35);
}

// Get the beat value of the rest
double HalfRest::getBeatValue()
{
    return 2;
}
