#include "mainwindow.h"

#include <QApplication>
#include "teststaff.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TestStaff test;
    QTest::qExec(&test, argc, argv);

    MainWindow w;

    w.show();
    return a.exec();
}
