#ifndef BARLINEMANAGER_H
#define BARLINEMANAGER_H

#include "rhythmsymbol.h"

#include <QWidget>
#include <QVector>
#include <QBoxLayout>

class BarLineManager : public QWidget
{
    Q_OBJECT
public:
    explicit BarLineManager(QWidget *parent = nullptr);
    static void updateBarLines(QVector<RhythmSymbol*> notes, QBoxLayout *layout, int signature);
    static void addBarLine(int index, QBoxLayout *layout);

signals:
};

#endif // BARLINEMANAGER_H