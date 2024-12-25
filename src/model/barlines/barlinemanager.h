#ifndef BARLINEMANAGER_H
#define BARLINEMANAGER_H

#include "rhythmsymbol.h"
#include "tablature.h"

#include <QWidget>
#include <QVector>
#include <QBoxLayout>

class BarLineManager : public QWidget
{
    Q_OBJECT
public:
    explicit BarLineManager(QWidget *parent = nullptr);
    static int updateBarLines(const QVector<RhythmSymbol*> &notes, QBoxLayout *layout, int signature, Tablature *tab);
    static int updateBarLines(const QVector<RhythmSymbol*> &notes, QBoxLayout *layout, int signature, Staff *staff);
    static int updateBarLines(const QVector<RhythmSymbol*> &notes, QBoxLayout *layout, int signature, Tablature *tab, Staff *staff);

signals:
};

#endif // BARLINEMANAGER_H
