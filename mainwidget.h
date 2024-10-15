#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "mainwindow.h"
#include "containerwidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(MainWindow *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    MainWindow *mainWindow;
    ContainerWidget *container;

signals:
};

#endif // MAINWIDGET_H
