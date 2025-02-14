#ifndef REGISTERUI_H
#define REGISTERUI_H

#include "mainwindow.h"
#include "authmanager.h"
#include "transitions.h"
#include "uiwidgets.h"
#include <QWidget>
#include <QThread>

// RegisterAccount Class
class RegisterUI : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterUI(MainWindow *parent = nullptr, AuthManager *authManager = nullptr);
    void removeText();
    void addErrorMessage(const QString &message);
    void removeErrorMessage(int wait);
    void invalidInput(TextField *fieldParent, const QString &message);

private:
    MainWindow *mainWindow;
    QVBoxLayout *widgetLayout;
    QLabel *registerSuccess = nullptr;
    QTimer *successMessageTimer = nullptr;
    QLabel *errorMessage = nullptr;
    QTimer *errorMessageTimer = nullptr;
    QLayout *errorLayout = nullptr;
    QSpacerItem *registerSpacer = nullptr;
    AuthManager *authManager;
    TextField *email;
    TextField *username;
    TextField *password;
    Transitions *transition;

private slots:
    void registerSlot();
};

#endif // REGISTERUI_H
