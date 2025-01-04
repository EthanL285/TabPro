#include "teststaff.h"
#include "tabprocontroller.h"
#include "crotchet.h"
#include "quaver.h"
#include "quarterrest.h"
#include "eighthrest.h"

TestStaff::TestStaff(QObject *parent)
    : QObject{parent}
{}

// Test for adding notes to staff
void TestStaff::testAddNote()
{
    // [C, C, Q, Q]
    TabProController *controller = new TabProController();

    // Add two crotchets
    controller->addNote(0, 0);
    controller->addNote(1, 2);

    QVector<RhythmSymbol*> notes = controller->getNotes();
    QCOMPARE(notes.size(), 2);
    QVERIFY(qobject_cast<Crotchet*>(notes[0]) != nullptr);
    QVERIFY(qobject_cast<Crotchet*>(notes[1]) != nullptr);

    // Add two quavers
    controller->changeSelectedNote(NoteType::Quaver);
    controller->addNote(2, 3);
    controller->addNote(3, 4);

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

    // Add two crotchets and quavers
    controller->addNote(0, 0);
    controller->addNote(1, 2);
    controller->changeSelectedNote(NoteType::Quaver);
    controller->addNote(2, 3);
    controller->addNote(3, 4);

    // Remove a quaver
    controller->removeNote();
    QVector<RhythmSymbol*> notes = controller->getNotes();
    QCOMPARE(notes.size(), 3);

    delete controller;
}

