#include "teststaff.h"
#include "tabprocontroller.h"
#include "crotchet.h"
#include "quaver.h"
#include "quarterrest.h"
#include "eighthrest.h"
#include "barline.h"

TestStaff::TestStaff(QObject *parent)
    : QObject{parent}
{}

// Test for adding notes to staff
void TestStaff::testAddNote()
{
    // [C, C]
    TabProController *controller = new TabProController();
    controller->createTab("2:C");

    QVector<RhythmSymbol*> notes = controller->getNotes();
    QCOMPARE(notes.size(), 2);
    QVERIFY(qobject_cast<Crotchet*>(notes[0]) != nullptr);
    QVERIFY(qobject_cast<Crotchet*>(notes[1]) != nullptr);

    // [C, C, Q, Q]
    controller->changeSelectedNote(NoteType::Quaver);
    controller->addNote(0, 0, 2);

    notes = controller->getNotes();
    QCOMPARE(notes.size(), 4);
    QVERIFY(qobject_cast<Quaver*>(notes[2]) != nullptr);
    QVERIFY(qobject_cast<Quaver*>(notes[3]) != nullptr);

    delete controller;
}

// Test for removing notes from staff
void TestStaff::testRemoveNote()
{
    // [C, C, Q, Q]
    TabProController *controller = new TabProController();
    controller->createTab("2:C,2:Q");

    // [C, C, Q]
    controller->removeNote(1);
    QVector<RhythmSymbol*> notes = controller->getNotes();
    QCOMPARE(notes.size(), 3);
    QVERIFY(qobject_cast<Crotchet*>(notes[0]) != nullptr);
    QVERIFY(qobject_cast<Crotchet*>(notes[1]) != nullptr);
    QVERIFY(qobject_cast<Quaver*>(notes[2]) != nullptr);

    // [C, CR, QR]
    controller->moveLeft(1);
    controller->removeNote(1);
    controller->moveLeft(1);
    controller->removeNote(1);

    notes = controller->getNotes();
    QCOMPARE(notes.size(), 3);
    QVERIFY(qobject_cast<Crotchet*>(notes[0]) != nullptr);
    QVERIFY(qobject_cast<QuarterRest*>(notes[1]) != nullptr);
    QVERIFY(qobject_cast<EighthRest*>(notes[2]) != nullptr);

    delete controller;
}

// Test for basic barline behaviour
void TestStaff::testBarLineBasic()
{
    // [C, C, C, C | Q, Q, Q, Q, Q, Q, Q, Q | Q, Q, C, C, Q, Q |]
    TabProController *controller = new TabProController();
    controller->createTab("4:C,8:Q,2:Q,2:C,2:Q");

    QVERIFY(qobject_cast<BarLine*>(controller->getStaffItem(4)) != nullptr);
    QVERIFY(qobject_cast<BarLine*>(controller->getStaffItem(13)) != nullptr);
    QVERIFY(qobject_cast<BarLine*>(controller->getStaffItem(20)) != nullptr);

    delete controller;
}

// Test for complex barline behaviour
void TestStaff::testBarLineComplex()
{
    // [C, C, C, Q, - | C, C, C, C | Q, Q, C, C, C |]
    TabProController *controller = new TabProController();
    controller->createTab("3:C,1:Q,4:C,2:Q,3:C");

    QVERIFY(qobject_cast<BarLine*>(controller->getStaffItem(5)) != nullptr);
    QVERIFY(qobject_cast<BarLine*>(controller->getStaffItem(10)) != nullptr);
    QVERIFY(qobject_cast<BarLine*>(controller->getStaffItem(16)) != nullptr);
    QVERIFY(qobject_cast<EighthRest*>(controller->getStaffItem(4)) != nullptr);

    // [C, C, C, Q, - | C, C, C, Q, Q, | C, C, C]
    controller->moveLeft(6);
    controller->removeNote(2);

    QVERIFY(qobject_cast<BarLine*>(controller->getStaffItem(5)) != nullptr);
    QVERIFY(qobject_cast<BarLine*>(controller->getStaffItem(11)) != nullptr);

    delete controller;
}

// Test for note replacement
void TestStaff::testNoteReplacementBasic()
{
    // [C, C, Q, Q]
    TabProController *controller = new TabProController();
    controller->createTab("2:C,2:Q");

    // [C, Q, Q, C]
    controller->moveLeft(1);
    controller->changeSelectedNote(NoteType::Crotchet);
    controller->addNote(0, 0, 1);
    controller->moveLeft(2);
    controller->changeSelectedNote(NoteType::Quaver);
    controller->addNote(0, 0, 1);

    QVector<RhythmSymbol*> notes = controller->getNotes();
    QCOMPARE(notes.size(), 4);
    QVERIFY(qobject_cast<Quaver*>(notes[1]) != nullptr);
    QVERIFY(qobject_cast<Crotchet*>(notes[3]) != nullptr);

    delete controller;
}

// Test for note replacement exceeding measure
void TestStaff::testNoteReplacementExceedsMeasure()
{
    // [Q, Q, C, Q, Q, C |]
    TabProController *controller = new TabProController();
    controller->createTab("2:Q,1:C,2:Q,1:C");

    // [Q, Q, C, Q, /(C), C |] -> [Q, Q, C, Q, C, QR |]
    controller->moveLeft(2);
    controller->addNote(0, 0, 1);

    QVERIFY(qobject_cast<Quaver*>(controller->getStaffItem(0)) != nullptr);
    QVERIFY(qobject_cast<Quaver*>(controller->getStaffItem(1)) != nullptr);
    QVERIFY(qobject_cast<Crotchet*>(controller->getStaffItem(2)) != nullptr);
    QVERIFY(qobject_cast<Quaver*>(controller->getStaffItem(3)) != nullptr);
    QVERIFY(qobject_cast<Crotchet*>(controller->getStaffItem(4)) != nullptr);
    QVERIFY(qobject_cast<EighthRest*>(controller->getStaffItem(5)) != nullptr);

    delete controller;
}

/* LEGEND
 * C = Crotchet
 * Q = Quaver
 * - = Left empty
 * /(x) = Replaced with x
 * -> = Equivalent to
 *
 */
