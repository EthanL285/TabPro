#include "passwordui.h"
#include "loginui.h"
#include "authmanager.h"

#include <QTime>

PasswordUI::PasswordUI(MainWindow *parent) : QWidget(parent), mainWindow(parent)
{
    setContentsMargins(0, 0, 0, 22);

    // Add rectangle layout
    QVBoxLayout *rectangleLayout = new QVBoxLayout(this);
    rectangleLayout->setAlignment(Qt::AlignHCenter);
    rectangleLayout->setSpacing(20);
    rectangleLayout->setContentsMargins(0, 70, 0, 50);

    // Add register box
    RectangleWidget *passwordBox = new RectangleWidget(this);
    rectangleLayout->addWidget(passwordBox);

    // Add widget layout
    widgetLayout = new QVBoxLayout(passwordBox);
    widgetLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    widgetLayout->setContentsMargins(40, 30, 40, 172);
    widgetLayout->setSpacing(30);

    // Add icon
    QPixmap appIcon(":/app/app/AppIconStripped.png");
    QLabel *iconLabel = new ResizableImageLabel(appIcon, this);

    iconLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    iconLabel->setMaximumSize(234, 234);

    widgetLayout->addWidget(iconLabel);
    widgetLayout->setAlignment(iconLabel, Qt::AlignHCenter);

    QSpacerItem *verticalSpacer1 = new QSpacerItem(0, 10, QSizePolicy::Fixed, QSizePolicy::Fixed);
    widgetLayout->addItem(verticalSpacer1);

    // Add title
    QHBoxLayout *titleLayout = new QHBoxLayout();
    widgetLayout->addLayout(titleLayout);
    titleLayout->setSpacing(23);

    QFrame *lineFrameLeft = new QFrame();
    lineFrameLeft->setFrameShape(QFrame::HLine);
    lineFrameLeft->setFrameShadow(QFrame::Sunken);
    titleLayout->addWidget(lineFrameLeft);

    QLabel *title = new QLabel("Set Password");
    title->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    title->setStyleSheet("color: white; font: 29pt Moon;");
    titleLayout->addWidget(title);

    QFrame *lineFrameRight = new QFrame();
    lineFrameRight->setFrameShape(QFrame::HLine);
    lineFrameRight->setFrameShadow(QFrame::Sunken);
    titleLayout->addWidget(lineFrameRight);

    // Add information
    info = new QLabel("Enter your account email address and we will share a 6 digit<br>verification code to reset your password");
    info->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    info->setStyleSheet("color: gray; font: 11pt Muli;");
    info->setAlignment(Qt::AlignCenter);
    widgetLayout->addWidget(info);
    widgetLayout->setAlignment(info, Qt::AlignHCenter);

    // Create text fields
    emailParent = new TextField("Email", ":/field/field/Email.png", passwordBox);
    emailParent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    widgetLayout->addWidget(emailParent);

    // Add send button
    QSpacerItem *verticalSpacer2 = new QSpacerItem(0, 10, QSizePolicy::Fixed, QSizePolicy::Fixed);
    widgetLayout->addItem(verticalSpacer2);

    sendButton = new MainButton("Send", passwordBox);
    widgetLayout->addWidget(sendButton);
    connect(sendButton, &QPushButton::clicked, this, &PasswordUI::sendResetEmail);

    // Add register label
    QHBoxLayout *registerLayout = new QHBoxLayout();
    widgetLayout->addLayout(registerLayout);
    registerLayout->setAlignment(Qt::AlignHCenter);
    registerLayout->setSpacing(0);

    QLabel *signup = new QLabel("Return back to ");
    signup->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    signup->setStyleSheet("color: gray; font: Italic 11pt Muli;");
    registerLayout->addWidget(signup);

    QLabel *reg = new ClickableLabel("Login", mainWindow);
    reg->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    reg->setStyleSheet("color: white; font: 11pt Muli;");
    reg->setCursor(Qt::PointingHandCursor);
    registerLayout->addWidget(reg);

    // Connect returnPressed() signal to the click() slot of button
    QLineEdit *email = emailParent->findChild<QLineEdit*>("field");
    connect(email, &QLineEdit::returnPressed, sendButton, &QPushButton::click);
}

// Send reset email to user
void PasswordUI::sendResetEmail()
{
    email = emailParent->findChild<QLineEdit*>("field");

    // Empty email field
    if (emailParent->emptyFieldCheck(email))
    {
        addErrorMessage("\u2717 Please fill in all required fields", 153, "rgb(237, 67, 55)");
        return;
    }
    // Email is not registered
    if (!AuthManager::instance().isRegisteredEmail(email->text()))
    {
        emailParent->setRedBorder(true);
        email->setFocus();
        addErrorMessage(userStatusMap[UserStatus::EmailNotFound], 153, "rgb(237, 57, 55)");
        return;
    }
    // Send verification code to user email
    AuthManager::instance().sendVerificationEmail(email->text());
    disableEmailField(true);
    addErrorMessage("⏳ Connecting to SMTP server...", 152, "gray");
}

// Email successfully delivered
void PasswordUI::onEmailSentSuccess()
{
    // Display verification page
    pageNumber = 1;
    displayVerificationPage();
    info->setText("Please enter the 6 digit verification code that has been sent to<br><span style=\"color:white;\">" + email->text() + "</span>");

    // Clear text from field
    userEmail = email->text();
    email->clear();
}

// Displays the verification page
void PasswordUI::displayVerificationPage()
{
    // Create continue button
    continueButton = new MainButton("Continue");
    connect(continueButton, &QPushButton::clicked, this, &PasswordUI::checkUserInput);

    // Create code fields
    codeParent = new QWidget(this);
    QHBoxLayout *codeLayout = new QHBoxLayout(codeParent);

    codeParent->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    codeLayout->setSpacing(20);
    codeLayout->setAlignment(Qt::AlignHCenter);

    QLineEdit *firstField;

    for (int i = 0; i < 6; ++i) {
        bool isLastField = (i == 5);
        QLineEdit* field = createCodeField(isLastField);
        codeLayout->addWidget(field);

        // Save reference to the first field
        if (i == 0) {
            firstField = field;
        }

        // Add field to the list
        codeFields.append(field);
    }

    // Add widgets to layout
    widgetLayout->removeWidget(emailParent);
    widgetLayout->removeWidget(sendButton);
    widgetLayout->insertWidget(4, codeParent);
    widgetLayout->insertWidget(6, continueButton);
    widgetLayout->setAlignment(codeParent, Qt::AlignHCenter);
    widgetLayout->setContentsMargins(40, 30, 40, 144);

    emailParent->setVisible(false);
    sendButton->setVisible(false);
    firstField->setFocus();
}

// Checks user input to see if it matches the verification code sent
void PasswordUI::checkUserInput()
{
    // Incomplete fields
    if (!allCodeFieldsFilled())
    {
        addErrorMessage(QString::fromUtf8("\u2717 ") + "Please fill in all required fields", 125, "rgb(237, 67, 55)");
        return;
    }
    // Invalid code
    QString userInput = getUserInputCode();
    if (userInput != AuthManager::instance().getVerificationCode())
    {
        addErrorMessage(QString::fromUtf8("\u2717 ") + "Incorrect verification code. Please try again", 125, "rgb(237, 67, 55)");
        return;
    }
    // Change page
    info->setText("Enter your account email address and we will share a 6 digit<br>verification code to reset your password");
    removeErrorMessage(0, 144);
    QTimer::singleShot(0, this, &PasswordUI::displayResetPasswordPage);
}

// Displays the reset passsword page
void PasswordUI::displayResetPasswordPage()
{
    pageNumber = 2;

    // Create main button and password fields
    confirmButton = new MainButton("Confirm");
    connect(confirmButton, &QPushButton::clicked, this, &PasswordUI::checkUserPassword);

    newPasswordParent = new TextField("New Password", ":/field/field/Lock.png", this);
    confirmNewPasswordParent = new TextField("Confirm Password", ":/field/field/Lock.png", this);

    newPasswordParent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    confirmNewPasswordParent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

     // Connect returnPressed() signal to the click() slot of button
    QLineEdit *newPasswordField = newPasswordParent->findChild<QLineEdit*>("field");
    QLineEdit *confirmPasswordField= this->confirmNewPasswordParent->findChild<QLineEdit*>("field");
    connect(newPasswordField, &QLineEdit::returnPressed, confirmButton, &QPushButton::click);
    connect(confirmPasswordField, &QLineEdit::returnPressed, confirmButton, &QPushButton::click);

    // Add widgets to layout
    widgetLayout->removeWidget(info);
    widgetLayout->removeWidget(codeParent);
    widgetLayout->removeWidget(continueButton);
    widgetLayout->insertWidget(3, newPasswordParent);
    widgetLayout->insertWidget(4, confirmNewPasswordParent);
    widgetLayout->insertWidget(6, confirmButton);
    widgetLayout->setAlignment(codeParent, Qt::AlignHCenter);
    widgetLayout->setContentsMargins(40, 30, 40, 160);

    info->setVisible(false);
    codeParent->setVisible(false);
    continueButton->setVisible(false);
}

// Checks users new password to see if it's valid
void PasswordUI::checkUserPassword()
{
    // User has already reset their password
    if (userHasResetPassword)
    {
        addErrorMessage(QString::fromUtf8("\u2717 ") + "Password recently reset. Return back to login", 141, "rgb(237, 67, 55)");
        if (resetSuccess)
        {
            widgetLayout->removeWidget(resetSuccess);
            delete resetSuccess;
            resetSuccess = nullptr;
        }
        return;
    }
    // Retrieve fields from parent widgets
    QLineEdit *newPasswordField = newPasswordParent->findChild<QLineEdit*>("field");
    QLineEdit *confirmPasswordField= this->confirmNewPasswordParent->findChild<QLineEdit*>("field");

    // Check for empty fields
    bool fieldEmpty = false;
    fieldEmpty = this->confirmNewPasswordParent->emptyFieldCheck(confirmPasswordField) || fieldEmpty;
    fieldEmpty = this->newPasswordParent->emptyFieldCheck(newPasswordField) || fieldEmpty;

    if (fieldEmpty)
    {
        addErrorMessage(QString::fromUtf8("\u2717 ") + "Please fill in all required fields", 141, "rgb(237, 67, 55)");
        return;
    }
    // Passwords do not match
    if  (newPasswordField->text() != confirmPasswordField->text())
    {
        newPasswordField->setFocus();
        newPasswordParent->setRedBorder(true);
        confirmNewPasswordParent->setRedBorder(true);
        addErrorMessage(QString::fromUtf8("\u2717 ") + "Passwords do not match. Please try again", 141, "rgb(237, 67, 55)");
        return;
    }
    // Invalid password
    PasswordStatus status = AuthManager::instance().verifyPassword(newPasswordField->text());
    if (status != PasswordStatus::Valid)
    {
        newPasswordField->setFocus();
        newPasswordParent->setRedBorder(true);
        confirmNewPasswordParent->setRedBorder(true);
        addErrorMessage(passwordStatusMap[status], 141, "rgb(237, 67, 55)");
        return;
    }
    // Update user's password
    AuthManager::instance().updateUserPassword(userEmail, newPasswordField->text());
    userHasResetPassword = true;

    // Reset widgets
    removeErrorMessage(0, 111);
    widgetLayout->setContentsMargins(40, 30, 40, 111);

    newPasswordParent->setRedBorder(false);
    confirmNewPasswordParent->setRedBorder(false);
    newPasswordField->clear();
    confirmPasswordField->clear();

    // Add success message
    resetSuccess = new QLabel(QString::fromUtf8("\u2713 ") + "Password successfully reset");
    resetSuccess->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    resetSuccess->setStyleSheet("color: rgb(114, 191, 106); font: 11pt Muli;");
    widgetLayout->addWidget(resetSuccess);
    widgetLayout->setAlignment(resetSuccess, Qt::AlignHCenter);
}

// Changes verification page back to send email page
void PasswordUI::removeVerificationPage()
{
    widgetLayout->removeWidget(codeParent);
    widgetLayout->removeWidget(continueButton);
    widgetLayout->insertWidget(4, emailParent);
    widgetLayout->insertWidget(6, sendButton);
    widgetLayout->setContentsMargins(40, 30, 40, 172);
    info->setText("Enter your account email address and we will share a 6 digit<br>verification code to reset your password");

    emailParent->setVisible(true);
    sendButton->setVisible(true);
    delete codeParent;
    delete continueButton;

    codeFields.clear();
    pageNumber = 0;
}

// Changes reset password page back to send email page
void PasswordUI::removeResetPasswordPage()
{
    widgetLayout->removeWidget(newPasswordParent);
    widgetLayout->removeWidget(confirmNewPasswordParent);
    widgetLayout->removeWidget(confirmButton);
    widgetLayout->insertWidget(3, info);
    widgetLayout->insertWidget(4, emailParent);
    widgetLayout->insertWidget(6, sendButton);
    widgetLayout->setContentsMargins(40, 30, 40, 172);

    emailParent->setVisible(true);
    sendButton->setVisible(true);
    info->setVisible(true);
    delete newPasswordParent;
    delete confirmNewPasswordParent;
    delete confirmButton;

    if (resetSuccess != nullptr)
    {
        widgetLayout->removeWidget(resetSuccess);
        delete resetSuccess;
        resetSuccess = nullptr;
    }

    codeFields.clear();
    userHasResetPassword = false;
    pageNumber = 0;
}

// Add error message to UI
void PasswordUI::addErrorMessage(const QString &message, int bottomMargin, const QString &colour)
{
    // Error message does not exist
    if (errorMessage == nullptr)
    {
        // Create and add new layout with error message
        errorLayout = new QVBoxLayout();
        QWidget *emailField = widgetLayout->itemAt(4)->widget();
        QWidget *sendButton = widgetLayout->itemAt(6)->widget();
        verticalSpacer = static_cast<QSpacerItem*>(widgetLayout->itemAt(5));

        errorMessage = new QLabel(message);
        errorMessage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        errorMessage->setStyleSheet("color: " + colour + "; font: 11pt Muli;");

        widgetLayout->removeWidget(emailField);
        widgetLayout->removeWidget(sendButton);
        widgetLayout->removeItem(verticalSpacer);

        errorLayout->addWidget(emailField);
        errorLayout->addWidget(errorMessage);
        errorLayout->addWidget(sendButton);

        errorLayout->setContentsMargins(0, 0, 0, 0);
        errorLayout->setSpacing(20);

        // This doesn't matter for text fields, but for code field it does since it does not expand horizontally
        if (pageNumber == 1)
        {
            errorLayout->setAlignment(emailField, Qt::AlignHCenter);
        }

        widgetLayout->insertLayout(4, errorLayout);
        widgetLayout->setContentsMargins(40, 30, 40, bottomMargin);

        // Bottom margin for 1st page: 153
        // Bottom margin for 2nd Page: 125
    }
    // Error message already exists
    else
    {
        errorMessage->setText(message);
        errorMessage->setStyleSheet("color: " + colour + "; font: 11pt Muli;");
    }
}

// Creates a field for code input
QLineEdit *PasswordUI::createCodeField(bool isLastField)
{
    QLineEdit *codefield = new QLineEdit(this);
    codefield->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    codefield->setAlignment(Qt::AlignCenter);
    codefield->setFixedSize(55, 60);
    codefield->setMaxLength(1);
    codefield->setStyleSheet
    (
        "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(96, 94, 92, 100), stop: 1 rgba(32, 31, 30, 200));"
        "border: none;"
        "border-radius: 5px;"
        "color: white;"
        "font: bold 20pt Muli;"
        "padding-left: 6px;"
        "padding-right: 6px;"
    );

    // Set validator to accept only integers
    QIntValidator *validator = new QIntValidator(0, 9, this);
    codefield->setValidator(validator);

    // Connect textChanged signal to a lambda function that sets focus to the next QLineEdit
    connect(codefield, &QLineEdit::textChanged, this, [codefield, this, isLastField](const QString &text)
    {
        if (!text.isEmpty() && !isLastField)
        {
            // Set focus on next QLineEdit
            QWidget *nextWidget = codefield->nextInFocusChain();
            if (nextWidget && nextWidget->isWidgetType())
            {
                nextWidget->setFocus();
            }
        }
    });
    // Connect returnPressed() signal to click() slot of button
    connect(codefield, &QLineEdit::returnPressed, continueButton, &QPushButton::click);

    return codefield;
}

// Checks if all code fields have been filled
bool PasswordUI::allCodeFieldsFilled()
{
    for (QLineEdit* field : codeFields)
    {
        if (field->text().isEmpty())
        {
            return false;
        }
    }
    return true;
}

// Gets the code user has inputted
QString PasswordUI::getUserInputCode()
{
    QString userInput;

    for (QLineEdit* field : codeFields)
    {
        userInput.append(field->text());
    }
    return userInput;
}

// Delete error message after 'wait' ms
void PasswordUI::removeErrorMessage(int wait, int bottomMargin)
{
    // Return if no error message
    if (!errorMessage || !errorLayout) return;

    // Stop existing active timer
    if (errorMessageTimer && errorMessageTimer->isActive())
    {
        errorMessageTimer->stop();
    }
    // Create a new timer
    errorMessageTimer = new QTimer(this);
    errorMessageTimer->setSingleShot(true);

    // Remove error message from widget once transition ends (timer expires)
    connect(errorMessageTimer, &QTimer::timeout, this, [this, bottomMargin]() {
        QWidget *emailField = errorLayout->itemAt(0)->widget();
        QWidget *sendButton = errorLayout->itemAt(2)->widget();

        errorLayout->removeWidget(emailField);
        errorLayout->removeWidget(errorMessage);
        errorLayout->removeWidget(sendButton);
        widgetLayout->removeItem(errorLayout);

        emailParent->setRedBorder(false);

        // Restore to original layout
        widgetLayout->insertWidget(4, emailField);
        widgetLayout->insertItem(5, verticalSpacer);
        widgetLayout->insertWidget(6, sendButton);
        widgetLayout->setContentsMargins(40, 30, 40, bottomMargin);

        delete errorMessage;
        delete errorLayout;
        errorMessage = nullptr;
        errorLayout = nullptr;

        delete errorMessageTimer;
        errorMessageTimer = nullptr;
    });
    // Start the timer
    errorMessageTimer->start(wait);
}

// Returns the current page of PasswordUI
int PasswordUI::getPageNumber()
{
    return pageNumber;

    /* Page Numbers
     * 0: Send email page
     * 1: Input code page
     * 2: Reset password page
     */
}

// Disables email field such that user's cannot edit
void PasswordUI::disableEmailField(bool disable)
{
    email->setReadOnly(disable);
}

// Slot for receiving a socket response
void PasswordUI::onSocketResponseReceived(const QString &response)
{
    disableEmailField(false);

    if (response != "Success")
        return addErrorMessage(response, 153, "rgb(237, 67, 55)");

    // Timer ensures error message is removed before user is redirected
    removeErrorMessage(0, 172);
    QTimer::singleShot(0, this, [this]()
    {
        disableEmailField(false);
        onEmailSentSuccess();
    });
}
