#include "barlinemanager.h"
#include "barline.h"
#include "rest.h"
#include "restfactory.h"

BarLineManager::BarLineManager(QWidget *parent)
    : QWidget{parent}
{}

// Updates the barlines of the given layout
// Returns the difference between the number of old and new barlines
int BarLineManager::updateBarLines(QVector<RhythmSymbol*> notes, QBoxLayout *layout, int signature, int barHeight)
{
    // Remove all barlines from layout
    int diff = 0;
    for (int i = layout->count() - 1; i >= 0; i--)
    {
        QWidget *widget = layout->itemAt(i)->widget();
        if (dynamic_cast<BarLine*>(widget))
        {
            layout->removeWidget(widget);
            delete widget;
            diff--;
        }
    }
    // Get positions of new barlines
    double beats = 0;
    QVector<int> newBarlineIdx;

    for (int i = 0; i < notes.size(); i++)
    {
        RhythmSymbol *symbol = dynamic_cast<RhythmSymbol*>(notes[i]);
        beats += symbol->getBeatValue();
        if (beats >= signature)
        {
            newBarlineIdx.append(i + 1);
            beats = 0;
        }
        /*
        else if (beats > signature)
        {
            Rest *rest = RestFactory::createRest(beats - signature);
            qDebug() << "Beats has exceeded signature";
        } */
    }
    // Add new barlines to layout
    int count = 1;
    for (int idx : newBarlineIdx)
    {
        layout->insertWidget(idx + count, new BarLine(barHeight));
        count++;
        diff++;
    }
    return diff;
}
