#ifndef PASSWORDUI_H
#define PASSWORDUI_H

#include "mainwindow.h"
#include "usermodel.h"
#include "uiwidgets.h"

// Reset Password Class
class PasswordUI : public QWidget
{
    Q_OBJECT
public:
    explicit PasswordUI(MainWindow *parent = nullptr, UserModel *usermodel = nullptr);

private:
    MainWindow *mainWindow;
    UserModel *usermodel;
    QVBoxLayout *widgetLayout;
    TextField *emailParent;

public slots:
    void sendResetEmail();
};

#endif // PASSWORDUI_H
