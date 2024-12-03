#ifndef BARLINE_H
#define BARLINE_H

#include "staffsymbol.h"

#include <QWidget>

class StaffBarLine : public StaffSymbol
{
    Q_OBJECT
public:
    explicit StaffBarLine();

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
};

#endif // BARLINE_H
