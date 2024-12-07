#ifndef BARLINE_H
#define BARLINE_H

#include "staffsymbol.h"

#include <QWidget>

class BarLine : public StaffSymbol
{
    Q_OBJECT
public:
    explicit BarLine();

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
};

#endif // BARLINE_H
