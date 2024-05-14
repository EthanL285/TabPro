#ifndef REGISTERUI_H
#define REGISTERUI_H

#include "mainwindow.h"
#include "usermodel.h"
#include "transitions.h"
#include <QWidget>
#include <QThread>

// RegisterAccount Class
class RegisterUI : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterUI(MainWindow *parent = nullptr);
    void removeText();
    void setRedBorder(QWidget *fieldWidget);

private:
    MainWindow *mainWindow;
    QVBoxLayout *widgetLayout;
    QLabel *registerSuccess = nullptr;
    UserModel *usermodel;
    QWidget *email;
    QWidget *username;
    QWidget *password;
    Transitions *transition;

private slots:
    void registerSlot();
};

#endif // REGISTERUI_H
