#ifndef BARLINE_H
#define BARLINE_H

#include "staffsymbol.h"

#include <QWidget>

class BarLine : public QWidget, StaffSymbol
{
    Q_OBJECT
public:
    explicit BarLine(QWidget *parent = nullptr);

signals:
};

#endif // BARLINE_H
