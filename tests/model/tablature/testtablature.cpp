#include "testtablature.h"
#include "tabprocontroller.h"

#include <QTest>

TestTablature::TestTablature(QObject *parent)
    : QObject{parent}
{}

// Test for adding notes to tab
void TestTablature::testAddNote()
{
    // [C, C]
    TabProController *controller = new TabProController();
    controller->createTab("2:C");
    controller->verifyTab("2:C");

    // [C, C, Q, Q]
    controller->setSelectedNote(NoteType::Quaver);
    controller->addNote(0, 0, 2);
    controller->verifyTab("2:C,2:Q");

    delete controller;
}

// Test for removing notes from tab
void TestTablature::testRemoveNote()
{
    // [C, C, Q, Q]
    TabProController *controller = new TabProController();
    controller->createTab("2:C,2:Q");

    // [C, C, Q]
    controller->removeNote(1);
    controller->verifyTab("2:C,1:Q");

    // [C, CR, QR]
    controller->moveLeft(1);
    controller->removeNote(1);
    controller->moveLeft(1);
    controller->removeNote(1);
    controller->verifyTab("1:C,1:CR,1:QR");

    delete controller;
}

// Test for clearing the tablature
void TestTablature::testClearTab()
{
    // Clear when selected note is last
    TabProController *controller = new TabProController();
    QVector<RhythmSymbol*> notes = controller->getNotes();
    controller->createTab("4:C");
    controller->clearTab();
    QCOMPARE(notes.size(), 0);

    // Clear when selected note is not last
    controller->createTab("8:Q");
    controller->moveLeft(4);
    controller->clearTab();
    QCOMPARE(notes.size(), 0);

    delete controller;
}

// Test for basic barline behaviour
void TestTablature::testBarLineBasic()
{
    // [C, C, C, C | Q, Q, Q, Q, Q, Q, Q, Q | Q, Q, C, C, Q, Q |]
    TabProController *controller = new TabProController();
    controller->createTab("4:C,8:Q,2:Q,2:C,2:Q");
    controller->verifyTab("4:C|8:Q|2:Q,2:C,2:Q|");

    delete controller;
}

// Test for complex barline behaviour
void TestTablature::testBarLineComplex()
{
    // [C, C, C, Q, - | C, C, C, C | Q, Q, C, C, C |]
    TabProController *controller = new TabProController();
    controller->createTab("3:C,1:Q,4:C,2:Q,3:C");
    controller->verifyTab("3:C,1:Q,1:QR|4:C|2:Q,3:C|");

    // [C, C, C, Q, - | C, C, C, Q, Q, | C, C, C]
    controller->moveLeft(6);
    controller->removeNote(2);
    controller->verifyTab("3:C,1:Q,1:QR|3:C,2:Q|3:C");

    delete controller;
}

// Test for note replacement
void TestTablature::testNoteReplacementBasic()
{
    // [C, C, Q, Q]
    TabProController *controller = new TabProController();
    controller->createTab("2:C,2:Q");

    // [C, /Q, Q, C]
    controller->moveLeft(1);
    controller->setSelectedNote(NoteType::Crotchet);
    controller->addNote(0, 0);
    controller->moveLeft(2);
    controller->setSelectedNote(NoteType::Quaver);
    controller->addNote(0, 0);
    controller->verifyTab("1:C,2:Q,1:C");

    delete controller;
}

// Test for note replacement exceeding measure
void TestTablature::testNoteReplacementExceedsMeasure()
{
    // Base Case: [Q, Q, C, Q, Q, C |]

    // CASE 1
    // [Q, Q, C, Q, /C, C |] -> [Q, Q, C, Q, C, QR |]
    TabProController *controller = new TabProController();
    controller->createTab("2:Q,1:C,2:Q,1:C");
    controller->moveLeft(2);
    controller->addNote(0, 0);
    controller->verifyTab("2:Q,1:C,1:Q,1:C,1:QR|");

    // CASE 2
    // [Q, Q, C, /C, Q, C |] -> [Q, Q, C, C, C |]
    controller->createTab("2:Q,1:C,2:Q,1:C");
    controller->moveLeft(3);
    controller->addNote(0, 0);
    controller->verifyTab("2:Q,3:C|");

    // CASE 3
    // [Q, /C, C, Q, Q, C |] -> [Q, Q, C, C, C |]
    controller->createTab("2:Q,1:C,2:Q,1:C");
    controller->moveLeft(5);
    controller->addNote(0, 0);
    controller->verifyTab("1:Q,1:C,1:QR,2:Q,1:C|");

    // CASE 4
    // [/C, Q, C, Q, Q, C |] -> [C, C, Q, Q, C|]
    controller->createTab("2:Q,1:C,2:Q,1:C");
    controller->moveLeft(6);
    controller->addNote(0, 0);
    controller->verifyTab("2:C,2:Q,1:C|");

    delete controller;
}

// Test for note replacement to lower beat value
void TestTablature::testNoteReplacementToLower()
{
    // CASE 1
    // [C, C, C, C | C]
    TabProController *controller = new TabProController();
    controller->createTab("5:C");

    // [C, C, C, \Q | C] -> [C, C, C, Q, QR | C]
    // Should not move crotchet from next measure
    controller->moveLeft(2);
    controller->setSelectedNote(NoteType::Quaver);
    controller->addNote(0, 0);
    controller->verifyTab("3:C,1:Q,1:QR|1:C");

    // Should not replace quarter rest
    controller->moveRight(1);
    controller->setSelectedNote(NoteType::Crotchet);
    controller->addNote(0, 0);
    controller->verifyTab("3:C,1:Q,1:QR|1:C");

    // Should not remove quarter rest (moves selected right)
    controller->removeNote(1);
    controller->verifyTab("3:C,1:Q,1:QR|1:C");

    // [C, C, C, Q, QR | \C] -> [C, C, C, Q, QR | Q]
    // Should not crash when editing first note in next measure
    controller->setSelectedNote(NoteType::Quaver);
    controller->addNote(0, 0);
    controller->verifyTab("3:C,1:Q,1:QR|1:Q");

    // CASE 2
    // [C, C, C, C | C, C, C, C]
    // [C, C, C, \Q | C, C, C, C] -> [C, C, C, Q, QR | C, C, C, C |]
    // Should not move any crotchets from next measure
    controller->createTab("8:C");
    controller->moveLeft(5);
    controller->setSelectedNote(NoteType::Quaver);
    controller->addNote(0, 0);
    controller->verifyTab("3:C,1:Q,1:QR|4:C|");

    delete controller;
}

// Test for chode mode
void TestTablature::testChordMode()
{
    // TODO: Fix tablature verification
    /*
    // [C, C]
    TabProController *controller = new TabProController();
    controller->createTab("2:C");

    // [C, C, Cc]
    controller->addChord({0, 1, 2, 3, 4, 5});
    controller->verifyTab("3:C");

    // [C, Cc, Cc]
    controller->moveLeft(1);
    controller->addChord({1, 2, 3, 4, 5, 6});
    controller->verifyTab("3:C");

    delete controller;
    */
}
