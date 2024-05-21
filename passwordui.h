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
    void addErrorMessage(const QString &message);
    void removeErrorMessage(int wait);
    QString generateVerificationCode();
    void onEmailSentSuccess();

private:
    MainWindow *mainWindow;
    UserModel *usermodel;
    QVBoxLayout *widgetLayout;
    TextField *emailParent;
    QLineEdit *email;
    QLabel *errorMessage = nullptr;
    QVBoxLayout *errorLayout = nullptr;
    QTimer *errorMessageTimer = nullptr;
    QSpacerItem *verticalSpacer = nullptr;
    QLabel *info;

public slots:
    void sendResetEmail();
};

#endif // PASSWORDUI_H
