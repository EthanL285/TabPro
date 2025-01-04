#ifndef TESTSTAFF_H
#define TESTSTAFF_H

#include <QObject>
#include <QTest>

class TestStaff : public QObject
{
    Q_OBJECT
public:
    explicit TestStaff(QObject *parent = nullptr);

private slots:
    void testAddNote();
    void testRemoveNote();

private:

};

#endif // TESTSTAFF_H
