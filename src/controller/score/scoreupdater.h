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
    static void updateBarLines(const QVector<RhythmSymbol*> &notes, Tablature *tab, Staff *staff, int beatsPerMeasure);

private:
    static bool isUpdating;
    static int removeBarLines(QHBoxLayout *layout);
    static int addBarLines(QHBoxLayout *layout, QVector<int> barLinePos, int layoutOffset, int barHeight);
    static QVector<int> getBarLinePos(const QVector<RhythmSymbol*> &notes, Tablature *tab, Staff *staff, int beatsPerMeasure);

signals:
};

#endif // SCOREUPDATER_H
