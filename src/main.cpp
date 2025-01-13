#include "mainwindow.h"
#include "teststaff.h"
#include "testtablature.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // TestStaff testStaff;
    // TestTablature testTab;

    // QTest::qExec(&testStaff, argc, argv);
    // QTest::qExec(&testTab, argc, argv);

    MainWindow w;

    w.show();
    return a.exec();
}
