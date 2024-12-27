#ifndef SCOREUPDATER_H
#define SCOREUPDATER_H

#include "rhythmsymbol.h"
#include "tablature.h"

#include <QWidget>
#include <QVector>
#include <QBoxLayout>

class ScoreUpdater : public QWidget
{
    Q_OBJECT
public:
    explicit ScoreUpdater(QWidget *parent = nullptr);
    static void update(const QVector<RhythmSymbol*> &notes, QBoxLayout *layout, int signature, Tablature *tab);
    static void update(const QVector<RhythmSymbol*> &notes, QBoxLayout *layout, int signature, Staff *staff);
    static void updateBarLines(const QVector<RhythmSymbol*> &notes, QBoxLayout *layout, int signature, Tablature *tab, Staff *staff);

signals:
};

#endif // SCOREUPDATER_H
