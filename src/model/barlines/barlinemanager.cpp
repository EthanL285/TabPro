#include "barlinemanager.h"
#include "barline.h"

#define STAFF_HEIGHT 28
#define TAB_HEIGHT 92

BarLineManager::BarLineManager(QWidget *parent)
    : QWidget{parent}
{}

// Updates the barlines of the tab
int BarLineManager::updateBarLines(const QVector<RhythmSymbol*> &notes, QBoxLayout *layout, int signature, Tablature *tab)
{
    return updateBarLines(notes, layout, signature, tab, nullptr);
}

// Updates the barlines of the staff
int BarLineManager::updateBarLines(const QVector<RhythmSymbol*> &notes, QBoxLayout *layout, int signature, Staff *staff)
{
    return updateBarLines(notes, layout, signature, nullptr, staff);
}

// Updates the barlines of the given widget
// Returns the difference between the old and new number of barlines in the layout
int BarLineManager::updateBarLines(const QVector<RhythmSymbol*> &notes, QBoxLayout *layout, int signature, Tablature *tab, Staff *staff)
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
        if (beats > signature)
        {
            (tab ? tab->insertRest(i) : staff->insertRest(i, beats - signature));
        }
        if (beats >= signature)
        {
            newBarlineIdx.append(i + 1);
            beats = 0;
        }
    }
    // Add new barlines to layout
    int count = 1;
    for (int idx : newBarlineIdx)
    {
        int barHeight = (tab) ? TAB_HEIGHT : STAFF_HEIGHT;
        layout->insertWidget(idx + count, new BarLine(barHeight));
        count++;
        diff++;
    }
    return diff;
}
