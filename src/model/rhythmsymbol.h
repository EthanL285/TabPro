#ifndef RYTHMSYMBOL_H
#define RYTHMSYMBOL_H

#include <QWidget>

class RhythmSymbol : public QWidget
{
    Q_OBJECT
public:
    virtual double getBeatValue() = 0;
    virtual void toggleSelect() = 0;
};

#endif // RYTHMSYMBOL_H
