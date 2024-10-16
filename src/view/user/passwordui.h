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
    void addErrorMessage(const QString &message, int bottomMargin, const QString &colour);
    void removeErrorMessage(int wait, int bottomMargin);
    QString generateVerificationCode();
    void onEmailSentSuccess();
    void displayVerificationPage();
    void displayResetPasswordPage();
    void removeVerificationPage();
    void removeResetPasswordPage();
    QLineEdit *createCodeField(bool isLastField = false);
    bool allCodeFieldsFilled();
    int getPageNumber();
    void disableEmailField(bool disable);
    QString getUserInputCode();
    void checkUserInput();
    void checkUserPassword();

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
    QPushButton *sendButton;
    QPushButton *continueButton;
    QPushButton *confirmButton;
    QList<QLineEdit*> codeFields;
    QWidget *codeParent;
    int pageNumber = 0;
    QString verificationCode;
    TextField *newPasswordParent;
    TextField *confirmNewPasswordParent;
    QLabel *resetSuccess = nullptr;
    bool userHasResetPassword = false;
    QString userEmail;

public slots:
    void sendResetEmail();
};

#endif // PASSWORDUI_H
