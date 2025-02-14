#ifndef REST_H
#define REST_H

#include "rhythmsymbol.h"

class Rest : public RhythmSymbol
{
    Q_OBJECT
public:
    void toggleSelect() override;

private:
    bool isSelected = false;
};

#endif // REST_H
