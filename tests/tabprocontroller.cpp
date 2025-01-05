#include "tabprocontroller.h"

#include <QTest>

TabProController::TabProController(QObject *parent)
    : QObject{parent}
{
    menu = new MenuBar();
    widget = new ContainerWidget(menu);
}

// Creates a tab from a comma-separated string where each entry is in the format "amount::NoteType"
// Each note is added at the default position (0, 0) to simplify testing
// Example input: "4:C,2:Q"
// - Adds a Crotchet 4 times
// - Adds a Quaver 2 times
void TabProController::createTab(QString tab)
{
    // Parse string
    QStringList pairs = tab.split(",");
    for (const QString &pair : pairs)
    {
        // Extract note type and amount
        QStringList parts = pair.split(":");
        NoteType type = stringToNoteType(parts[1].trimmed());
        int amount = parts[0].toInt();

        // Add corresponding note
        changeSelectedNote(type);
        addNote(0, 0, amount);
    }
}

// Compares the actual tab against a comma-separated string representing the expected tab
// Example input: "4:C,|,8:Q|"
// - 1st measure consists of 4 Crotchets
// - 2nd measure consists of 8 Quavers
void TabProController::verifyTab(QString expectedTab)
{
    // TODO: Find a way to include bar lines
    QVector<RhythmSymbol*> notes = getNotes();

    QStringList pairs = expectedTab.split(",");
    for (const QString &pair : pairs)
    {
        // Extract note type and amount
        QStringList parts = pair.split(":");
        NoteType type = stringToNoteType(parts[1].trimmed());
        int amount = parts[0].toInt();

    }
}

// Returns a vector of all the notes on the staff
QVector<RhythmSymbol*> TabProController::getNotes()
{
    return widget->getNotes();
}

// Returns the staff layout item at the given index
QWidget *TabProController::getStaffItem(int index)
{
    return widget->getStaffItem(index);
}

// Returns the tab layout item at the given index
QWidget *TabProController::getTabItem(int index)
{
    return widget->getTabItem(index);
}

// Adds a note to the tab and staff 'x' times
void TabProController::addNote(int string, int fret, int x)
{
    for (int i = 0; i < x; i++)
    {
        QTest::mouseClick(widget->getFretButton(string, fret), Qt::LeftButton);
    }
}

// Removes the currently selected note from the tab and staff 'x' times
void TabProController::removeNote(int x)
{
    for (int i = 0; i < x; i++)
    {
        QTest::mouseClick(widget->getUIButton("\u2715"), Qt::LeftButton);
    }
}

// Changes the currently selected note to the given type
void TabProController::changeSelectedNote(NoteType type)
{
    QTest::mouseClick(widget->getSelectedNoteButton(type), Qt::LeftButton);
}

// Moves the tab left 'x' times
void TabProController::moveLeft(int x)
{
    for (int i = 0; i < x; i++)
    {
        QTest::mouseClick(widget->getUIButton("left"), Qt::LeftButton);
    }
}

// Moves the tab right 'x' times
void TabProController::moveRight(int x)
{
    for (int i = 0; i < x; i++)
    {
        QTest::mouseClick(widget->getUIButton("right"), Qt::LeftButton);
    }
}

// Destructor
TabProController::~TabProController()
{
    delete widget;
    delete menu;
}


