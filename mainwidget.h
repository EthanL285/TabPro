#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "mainwindow.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(MainWindow *parent = nullptr);
    void toggleSidebar();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    MainWindow *mainWindow;
    QWidget *sidebar;
    QPropertyAnimation *sidebarAnimation;

signals:
};

#endif // MAINWIDGET_H
