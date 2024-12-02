#ifndef RYTHMSYMBOL_H
#define RYTHMSYMBOL_H

#include "staffsymbol.h"

class RhythmSymbol : public StaffSymbol
{
    Q_OBJECT
public:
    virtual double getBeatValue() = 0;
};

#endif // RYTHMSYMBOL_H
