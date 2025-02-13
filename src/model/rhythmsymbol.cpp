#include "rhythmsymbol.h"

// Slot for width change
void RhythmSymbol::onWidthChange(int newWidth, int prevWidth)
{
    setFixedWidth(newWidth);
    emit widthChanged(newWidth, prevWidth);
}


