#include "scoreupdater.h"
#include "barline.h"
#include "rest.h"
#include "restfactory.h"
#include "rhythmsymbol.h"
#include "tablature.h"

ScoreUpdater::ScoreUpdater(QWidget *parent)
    : QWidget{parent}
{}

bool ScoreUpdater::isUpdating = false;

// Visually updates the barlines on the tab/staff
void ScoreUpdater::updateBarLines()
{
    if (isUpdating) return;
    isUpdating = true;

    QHBoxLayout *staffLayout = Staff::getInstance()->getLayout();
    QHBoxLayout *tabLayout = Tablature::getInstance()->getLayout();

    // Remove all barlines from tab and staff
    int barLinesRemoved = removeBarLines(staffLayout);
    removeBarLines(tabLayout);

    // Get positions of new barlines
    QVector<int> newBarLinePos = getBarLinePos();

    // Add new barlines to tab and staff
    int barLinesAdded = addBarLines(staffLayout, newBarLinePos, Staff::LAYOUT_OFFSET, Staff::BARLINE_HEIGHT);
    addBarLines(tabLayout, newBarLinePos, Tablature::LAYOUT_OFFSET, Tablature::BARLINE_HEIGHT);

    // Update staff length
    Staff::getInstance()->updateLength(Tablature::DEFAULT_COLUMN_WIDTH, barLinesAdded - barLinesRemoved);
    isUpdating = false;
}

// Returns the number of barlines removed from the layout
int ScoreUpdater::removeBarLines(QHBoxLayout *layout)
{
    int count = 0;
    for (int i = layout->count() - 1; i >= 0; i--)
    {
        QWidget *widget = layout->itemAt(i)->widget();
        if (dynamic_cast<BarLine*>(widget))
        {
            layout->removeWidget(widget);
            delete widget;
            count++;
        }
    }
    return count;
}

// Returns the number of barlines added to the layout at the given positions
int ScoreUpdater::addBarLines(QHBoxLayout *layout, QVector<int> barLinePos, int layoutOffset, int barHeight)
{
    int count = 0;
    for (int i = 0; i < barLinePos.size(); i++)
    {
        layout->insertWidget(barLinePos[i] + layoutOffset + i, new BarLine(barHeight));
        count++;
    }
    return count;
}

// Returns the positions of new barlines
QVector<int> ScoreUpdater::getBarLinePos()
{
    double beats = 0;
    int beatsPerMeasure = Staff::getInstance()->getBeatsPerMeasure();

    QVector<int> newBarlinePos;
    QVector<RhythmSymbol*> notes = Staff::getInstance()->getNotes();

    for (int i = 0; i < notes.size(); i++)
    {
        RhythmSymbol *symbol = dynamic_cast<RhythmSymbol*>(notes[i]);
        beats += symbol->getBeatValue();
        if (beats > beatsPerMeasure)
        {
            Rest *rest = RestFactory::createRest(beats - beatsPerMeasure);
            Tablature::getInstance()->addColumn(i, Tablature::EMPTY_COLUMN, rest);
        }
        if (beats >= beatsPerMeasure)
        {
            newBarlinePos.append(i + 1);
            beats = 0;
        }
    }
    return newBarlinePos;
}
