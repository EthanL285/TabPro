#include "tabprocontroller.h"

#include <QTest>

TabProController::TabProController(QObject *parent)
    : QObject{parent}
{
    menu = new MenuBar();
    widget = new ContainerWidget(menu);
}

// Returns a vector of all the notes on the staff
QVector<RhythmSymbol*> TabProController::getNotes()
{
    return widget->getNotes();
}

// Adds a note to the tab and staff
void TabProController::addNote(int string, int fret)
{
    QTest::mouseClick(widget->getFretButton(string, fret), Qt::LeftButton);
}

// Removes the note at the given index from the tab and staff
void TabProController::removeNote()
{
    QTest::mouseClick(widget->getTechniqueButton("\u2715"), Qt::LeftButton);
}

// Changes the currently selected note to the given type
void TabProController::changeSelectedNote(NoteType type)
{
    QTest::mouseClick(widget->getSelectedNoteButton(type), Qt::LeftButton);
}

// Destructor
TabProController::~TabProController()
{
    delete widget;
}


