#ifndef TESTTABLATURE_H
#define TESTTABLATURE_H

#include <QObject>

class TestTablature : public QObject
{
    Q_OBJECT
public:
    explicit TestTablature(QObject *parent = nullptr);

private slots:
    void testAddNote();
    void testRemoveNote();
    void testClearTab();
    void testBarLineBasic();
    void testBarLineComplex();
    void testNoteReplacementBasic();
    void testNoteReplacementExceedsMeasure();

signals:
};

#endif // TESTTABLATURE_H
