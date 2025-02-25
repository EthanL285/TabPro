#include "halfrest.h"
#include "tablature.h"

HalfRest::HalfRest()
{
    setFixedWidth(Tablature::DEFAULT_COLUMN_WIDTH);
}

// Get the beat value of the rest
double HalfRest::getBeatValue()
{
    return 2;
}
