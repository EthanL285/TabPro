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
    void setRedBorder(QWidget *fieldWidget, bool setRed);
    void addErrorMessage(const QString &message);
    void removeErrorMessage(int wait);
    bool emptyFieldCheck(QWidget *fieldParent, QLineEdit *field);
    void invalidInput(QWidget *fieldParent, QString &message);

private:
    MainWindow *mainWindow;
    QVBoxLayout *widgetLayout;
    QLabel *registerSuccess = nullptr;
    QTimer *successMessageTimer = nullptr;
    QLabel *errorMessage = nullptr;
    QTimer *errorMessageTimer = nullptr;
    QLayout *errorLayout = nullptr;
    QSpacerItem *registerSpacer = nullptr;
    UserModel *usermodel;
    QWidget *email;
    QWidget *username;
    QWidget *password;
    Transitions *transition;

private slots:
    void registerSlot();
};

#endif // REGISTERUI_H
