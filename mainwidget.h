#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "mainwindow.h"
#include "sidebar.h"
#include "guitar.h"
#include "menubar.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(MainWindow *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    MainWindow *mainWindow;
    Sidebar *sidebar;
    Guitar *guitar;
    MenuBar *menubar;

signals:
};

#endif // MAINWIDGET_H
