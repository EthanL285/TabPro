#ifndef REGISTERUI_H
#define REGISTERUI_H

#include "mainwindow.h"
#include <QWidget>

// RegisterAccount Class
class RegisterUI : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterUI(MainWindow *parent = nullptr);

private:
    MainWindow *mainWindow;

private slots:
    void registerSlot();
};

#endif // REGISTERUI_H
