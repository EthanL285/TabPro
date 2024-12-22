#include "barlinemanager.h"
#include "note.h"
#include "barline.h"
#include "rest.h"
#include "restfactory.h"

BarLineManager::BarLineManager(QWidget *parent)
    : QWidget{parent}
{}

// Updates the barlines of the given layout
void BarLineManager::updateBarLines(QVector<RhythmSymbol*> notes, QBoxLayout *layout, int signature, int barHeight)
{
    QVector<int> currBarlinePos;
    QVector<int> newBarlinePos;

    // Get positions of new barlines
    double beats = 0;
    for (int i = 0; i < notes.size(); i++)
    {
        Note *note = dynamic_cast<Note*>(notes[i]);
        beats += note->getBeatValue();
        if (beats >= signature)
        {
            newBarlinePos.append(i + 1);
            beats = 0;
        }
        /*
        else if (beats > signature)
        {
            Rest *rest = RestFactory::createRest(beats - signature);
            qDebug() << "Beats has exceeded signature";
        } */
    }
    // Get positions of current barlines
    int idx = 0;
    for (int i = 1; i < layout->count(); i++)
    {
        QWidget *widget = layout->itemAt(i)->widget();
        if (dynamic_cast<BarLine*>(widget))
        {
            currBarlinePos.append(idx);
            continue;
        }
        idx++;
    }
    // Compare current and new positions
    for (int newPos : newBarlinePos)
    {
        if (!currBarlinePos.contains(newPos)) addBarLine(newPos, barHeight, layout);
    }
}

// Adds a barline at the given index in the layout
void BarLineManager::addBarLine(int index, int barHeight, QBoxLayout *layout)
{
    layout->addWidget(new BarLine(barHeight));
}
