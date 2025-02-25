#ifndef REST_H
#define REST_H

#include "rhythmsymbol.h"
#include "resttype.h"

class Rest : public RhythmSymbol
{
    Q_OBJECT
public:
    RestType getType();
    void toggleSelect() override;
    QJsonObject toJson() const override;

private:
    RestType type;
    bool isSelected = false;
};

#endif // REST_H
