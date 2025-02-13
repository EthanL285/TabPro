#include "tabprocontroller.h"
#include "toggleswitch.h"

#include <QTest>

#define NOTE_BUTTON "0\n\u2015\n\u2015\n\u2015\n\u2015\n\u2015"
#define REST_BUTTON "\u2015\n\u2015\n\u2015\n\u2015\n\u2015\n\u2015"

TabProController::TabProController(QObject *parent)
    : QObject{parent}
{
    widget = new ContainerWidget(nullptr);
}

// Returns a vector of all the notes on the staff
const QVector<RhythmSymbol*> &TabProController::getNotes()
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

//////////////////////////////////////////////////////////////////////
//                          TAB FUNCTIONS                           //
//////////////////////////////////////////////////////////////////////

// Creates a tab from a comma-separated string where each entry is in the format "amount::NoteType"
// Each note is added at the default position (0, 0) to simplify testing
// Example input: "4:C,2:Q"
// - Adds a Crotchet 4 times
// - Adds a Quaver 2 times
void TabProController::createTab(QString tab)
{
    clearTab();

    // Parse string
    QStringList pairs = tab.split(",");
    for (const QString &pair : pairs)
    {
        // Extract note type and amount
        QStringList parts = pair.split(":");
        NoteType type = stringToNoteType(parts[1].trimmed());
        int amount = parts[0].toInt();

        // Add corresponding note
        setSelectedNote(type);
        addNote(0, 0, amount);
    }
}

// Clears the tablature
void TabProController::clearTab()
{
    QTest::mouseClick(widget->getMenuButton("reset tab"), Qt::LeftButton);
}

//////////////////////////////////////////////////////////////////////
//                      VERIFICATION FUNCTIONS                      //
//////////////////////////////////////////////////////////////////////

// Verifies the tab by comparing the actual tab against a string representing the expected tab
// Example input: "4:C|8:Q|"
// - 1st measure consists of 4 Crotchets
// - 2nd measure consists of 8 Quavers
void TabProController::verifyTab(QString expectedTab)
{
    int idx = 0;
    int tabSize = widget->getLayoutSize();
    bool endsWithBarline = expectedTab.endsWith("|");

    // Parse string
    QStringList measures = expectedTab.split("|");
    if (endsWithBarline) measures.removeLast();

    for (int i = 0; i < measures.size(); i++)
    {
        QStringList pairs = measures[i].split(",");
        for (const QString &pair : pairs)
        {
            // Extract amount
            QStringList parts = pair.split(":");
            QString type = parts[1].trimmed();
            int amount = parts[0].toInt();

            // Check buttons
            for (int j = 0; j < amount; j++)
            {
                QVERIFY(idx < tabSize);
                QPushButton *button = qobject_cast<QPushButton*>(getTabItem(idx));
                QString expectedText = type.contains("R") ? REST_BUTTON : NOTE_BUTTON;
                QVERIFY(button);
                QCOMPARE(button->text(), expectedText);
                idx++;
            }
        }
        // Last measure
        if (i == measures.size() - 1 && !endsWithBarline) break;

        // Check barline
        QCOMPARE(getTabItem(idx)->metaObject()->className(), "BarLine");
        idx++;
    }
    // Expected tab length matches actual tab length
    QCOMPARE(idx, tabSize);
}

// Verifies the staff by comparing the actual tab against a string representing the expected tab
// Example input: "4:C|8:Q|"
// - 1st measure consists of 4 Crotchets
// - 2nd measure consists of 8 Quavers
void TabProController::verifyStaff(QString expectedTab)
{
    int idx = 0;
    int tabSize = widget->getLayoutSize();
    bool endsWithBarline = expectedTab.endsWith("|");

    QMap<QString, QString> map =
    {
        {"C", "Crotchet"},
        {"Q", "Quaver"},
        {"CR", "QuarterRest"},
        {"QR", "EighthRest"}
    };
    // Parse string
    QStringList measures = expectedTab.split("|");
    if (endsWithBarline) measures.removeLast();

    for (int i = 0; i < measures.size(); i++)
    {
        QStringList pairs = measures[i].split(",");
        for (const QString &pair : pairs)
        {
            // Extract note type and amount
            QStringList parts = pair.split(":");
            QString type = parts[1].trimmed();
            int amount = parts[0].toInt();

            // Check notes
            for (int j = 0; j < amount; j++)
            {
                QVERIFY(idx < tabSize);
                QCOMPARE(getStaffItem(idx)->metaObject()->className(), map[type]);
                idx++;
            }
        }
        // Last measure
        if (i == measures.size() - 1 && !endsWithBarline) break;

        // Check barline
        QCOMPARE(getStaffItem(idx)->metaObject()->className(), "BarLine");
        idx++;
    }
    // Expected tab length matches actual tab length
    QCOMPARE(idx, tabSize);
}

//////////////////////////////////////////////////////////////////////
//                         NOTE FUNCTIONS                           //
//////////////////////////////////////////////////////////////////////

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
        QTest::mouseClick(widget->getUIWidget("\u2715"), Qt::LeftButton);
    }
}

// Adds a chord to the tab
void TabProController::addChord(QVector<int> fretNumbers)
{
    setChordMode(true);
    for(int i = 0; i < Tablature::NUM_STRINGS; i++)
    {
        addNote(i, fretNumbers[i]);
    }
    setChordMode(false);
}

//////////////////////////////////////////////////////////////////////
//                         MOVE FUNCTIONS                           //
//////////////////////////////////////////////////////////////////////

// Moves the tab left 'x' times
void TabProController::moveLeft(int x)
{
    for (int i = 0; i < x; i++)
    {
        QTest::mouseClick(widget->getUIWidget("left"), Qt::LeftButton);
    }
}

// Moves the tab right 'x' times
void TabProController::moveRight(int x)
{
    for (int i = 0; i < x; i++)
    {
        QTest::mouseClick(widget->getUIWidget("right"), Qt::LeftButton);
    }
}

//////////////////////////////////////////////////////////////////////
//                         SET FUNCTIONS                            //
//////////////////////////////////////////////////////////////////////

// Sets the selected note to the given type
void TabProController::setSelectedNote(NoteType type)
{
    QTest::mouseClick(widget->getMenuButton(type), Qt::LeftButton);
}

// Sets the chord mode
void TabProController::setChordMode(bool enable)
{
    ToggleSwitch *chordSwitch = qobject_cast<ToggleSwitch*>(widget->getUIWidget("Chord Switch"));
    if (chordSwitch->isToggled() == enable) return;
    QTest::mouseClick(chordSwitch, Qt::LeftButton);
}

// Destructor
TabProController::~TabProController()
{
    delete widget;
}


