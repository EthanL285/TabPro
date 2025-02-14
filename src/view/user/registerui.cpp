#include "registerui.h"
#include "uiwidgets.h"
#include "loginui.h"

RegisterUI::RegisterUI(MainWindow *parent, AuthManager *authManager) : QWidget(parent), mainWindow(parent), authManager(authManager)
{
    setContentsMargins(0, 0, 0, 22);

    // Create Transitions object
    transition = new Transitions(this);

    // Add rectangle layout
    QVBoxLayout *rectangleLayout = new QVBoxLayout(this);
    rectangleLayout->setAlignment(Qt::AlignHCenter);
    rectangleLayout->setSpacing(20);
    rectangleLayout->setContentsMargins(0, 70, 0, 50);

    // Add register box
    RectangleWidget *registerBox = new RectangleWidget(this);
    rectangleLayout->addWidget(registerBox);

    // Add widget layout
    widgetLayout = new QVBoxLayout(registerBox);
    widgetLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    widgetLayout->setContentsMargins(40, 30, 40, 80);
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

    QFrame *lineFrameLeft = new QFrame();
    lineFrameLeft->setFrameShape(QFrame::HLine);
    lineFrameLeft->setFrameShadow(QFrame::Sunken);
    titleLayout->addWidget(lineFrameLeft);

    QLabel *title = new QLabel("Register");
    title->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    title->setStyleSheet("color: white; font: 30pt Moon;");
    titleLayout->addWidget(title);

    QFrame *lineFrameRight = new QFrame();
    lineFrameRight->setFrameShape(QFrame::HLine);
    lineFrameRight->setFrameShadow(QFrame::Sunken);
    titleLayout->addWidget(lineFrameRight);

    // Create text fields
    email = new TextField("Email", ":/field/field/Email.png", registerBox);
    username = new TextField("Username", ":/field/field/User.png", registerBox);
    password = new TextField("Password", ":/field/field/Lock.png", registerBox);

    email->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    username->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    password->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    widgetLayout->addWidget(email);
    widgetLayout->addWidget(username);
    widgetLayout->addWidget(password);

    // Add Register button
    QSpacerItem *verticalSpacer2 = new QSpacerItem(0, 10, QSizePolicy::Fixed, QSizePolicy::Fixed);
    widgetLayout->addItem(verticalSpacer2);

    QPushButton *registerButton = new MainButton("Sign Up", registerBox);
    widgetLayout->addWidget(registerButton);
    connect(registerButton, &QPushButton::clicked, this, &RegisterUI::registerSlot);

    // Add login back button
    QHBoxLayout *loginLayout = new QHBoxLayout();
    widgetLayout->addLayout(loginLayout);
    loginLayout->setAlignment(Qt::AlignHCenter);
    loginLayout->setSpacing(0);

    QLabel *accountExists = new QLabel("Already have an account?  ");
    accountExists->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    accountExists->setStyleSheet("color: gray; font: Italic 11pt Muli;");
    loginLayout->addWidget(accountExists);

    QLabel *login = new ClickableLabel("Login", mainWindow);
    login->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    login->setStyleSheet("color: white; font: 11pt Muli;");
    login->setCursor(Qt::PointingHandCursor);
    loginLayout->addWidget(login);

    // Connect the returnPressed() signal of text fields to the click() signal of button
    QLineEdit *email = this->email->findChild<QLineEdit*>("field");
    QLineEdit *username = this->username->findChild<QLineEdit*>("field");
    QLineEdit *password = this->password->findChild<QLineEdit*>("field");

    connect(email, &QLineEdit::returnPressed, registerButton, &QPushButton::click);
    connect(username, &QLineEdit::returnPressed, registerButton, &QPushButton::click);
    connect(password, &QLineEdit::returnPressed, registerButton, &QPushButton::click);
}

// RegisterUI Slot
void RegisterUI::registerSlot()
{
    // Retrieve fields from parent widgets
    QLineEdit *email = this->email->findChild<QLineEdit*>("field");
    QLineEdit *username = this->username->findChild<QLineEdit*>("field");
    QLineEdit *password = this->password->findChild<QLineEdit*>("field");

    // Check for empty fields
    bool fieldEmpty = false;
    fieldEmpty = this->password->emptyFieldCheck(password) || fieldEmpty;
    fieldEmpty = this->username->emptyFieldCheck(username) || fieldEmpty;
    fieldEmpty = this->email->emptyFieldCheck(email) || fieldEmpty;

    if (fieldEmpty)
    {
        addErrorMessage(QString::fromUtf8("\u2717 ") + "Please fill in all required fields");
        return;
    }
    // Validate credentials
    EmailStatus emailStatus = authManager->verifyEmail(email->text());
    if (emailStatus != EmailStatus::Valid)
    {
        invalidInput(this->email, emailStatusMap[emailStatus]);
        email->setFocus();
        return;
    }
    UsernameStatus usernameStatus = authManager->verifyUsername(username->text());
    if (usernameStatus != UsernameStatus::Valid)
    {
        invalidInput(this->username, usernameStatusMap[usernameStatus]);
        username->setFocus();
        return;
    }
    PasswordStatus passwordStatus = authManager->verifyPassword(password->text());
    if (passwordStatus != PasswordStatus::Valid)
    {
        invalidInput(this->password, passwordStatusMap[passwordStatus]);
        password->setFocus();
        return;
    }
    // Add user to database
    authManager->addUser(email->text(), username->text(), password->text());
    removeErrorMessage(0);

    // First time registering
    if (registerSuccess == nullptr)
    {
        // Add success text
        registerSuccess = new QLabel(QString::fromUtf8("\u2713 ") + "Account successfully created");
        registerSuccess->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        registerSuccess->setStyleSheet("color: rgb(114, 191, 106); font: 11pt Muli;");
        widgetLayout->setContentsMargins(40, 30, 40, 31);
        widgetLayout->addWidget(registerSuccess);
        widgetLayout->setAlignment(registerSuccess, Qt::AlignHCenter);
    }
    // Clear text from field
    email->clear();
    username->clear();
    password->clear();
}

// Creates error message for invalid inputs
void RegisterUI::invalidInput(TextField *fieldParent, const QString &message)
{
    if (message == "Valid") return;

    // Set error message and border
    fieldParent->setRedBorder(true);
    addErrorMessage(message);
}

// Create new layout containing password field, failure text and sign-in button
void RegisterUI::addErrorMessage(const QString &message)
{
    // Error message already exists
    if (errorMessage)
    {
        errorMessage->setText(message);
        return;
    }
    // Remove success message
    if (registerSuccess != nullptr)
    {
        widgetLayout->removeWidget(registerSuccess);
        delete registerSuccess;
        registerSuccess = nullptr;
    }
    // Create and add new layout with error message
    errorLayout = new QVBoxLayout();
    QWidget *registerButton = widgetLayout->itemAt(7)->widget();
    QWidget *passwordField = widgetLayout->itemAt(5)->widget();
    registerSpacer = static_cast<QSpacerItem*>(widgetLayout->itemAt(6));

    errorMessage = new QLabel(message);
    errorMessage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    errorMessage->setStyleSheet("color: rgb(237, 67, 55); font: 11pt Muli;");
    errorMessage->setContentsMargins(0, 20, 0, 20);

    errorLayout->addWidget(passwordField);
    errorLayout->addWidget(errorMessage);
    errorLayout->addWidget(registerButton);
    errorLayout->setContentsMargins(0, 0, 0, 0);
    errorLayout->setSpacing(0);

    // Replace widgets with new layout
    widgetLayout->removeWidget(registerButton);
    widgetLayout->removeWidget(passwordField);
    widgetLayout->removeItem(registerSpacer);
    widgetLayout->insertLayout(5, errorLayout);
    widgetLayout->setContentsMargins(40, 30, 40, 71);
}

// Delete register success text
void RegisterUI::removeText()
{
    if (!registerSuccess) return;

    // Stop existing active timer
    if (successMessageTimer && successMessageTimer->isActive())
    {
        successMessageTimer->stop();
    }
    // Create a new timer
    successMessageTimer = new QTimer(this);
    successMessageTimer->setSingleShot(true);

    // Delete success message once transition ends (timer expires)
    connect(successMessageTimer, &QTimer::timeout, this, [this]()
    {
        widgetLayout->removeWidget(registerSuccess);
        widgetLayout->setContentsMargins(40, 30, 40, 80);

        delete registerSuccess;
        registerSuccess = nullptr;
        successMessageTimer = nullptr;
    });
    successMessageTimer->start(500);
}

// Delete error message after 'wait' ms
void RegisterUI::removeErrorMessage(int wait)
{
    // Return if no error message
    if (!errorMessage || !errorLayout) return;

    // Stop any active timer
    if (errorMessageTimer && errorMessageTimer->isActive())
    {
        errorMessageTimer->stop();
    }
    // Create a new timer
    errorMessageTimer = new QTimer(this);
    errorMessageTimer->setSingleShot(true);

    // Remove error message from widget once transition ends (timer expires)
    connect(errorMessageTimer, &QTimer::timeout, this, [this]() {
        QWidget *passwordField = errorLayout->itemAt(0)->widget();
        QWidget *registerButton = errorLayout->itemAt(2)->widget();

        errorLayout->removeWidget(passwordField);
        errorLayout->removeWidget(errorMessage);
        errorLayout->removeWidget(registerButton);
        widgetLayout->removeItem(errorLayout);

        this->email->setRedBorder(false);
        this->username->setRedBorder(false);
        this->password->setRedBorder(false);

        // Restore to original layout
        widgetLayout->insertWidget(5, passwordField);
        widgetLayout->insertItem(6, registerSpacer);
        widgetLayout->insertWidget(7, registerButton);
        widgetLayout->setContentsMargins(40, 30, 40, 80);

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

// ClickableLabel Class
void ClickableLabel::backToLogin()
{
    mainWindow->redirectLogin();
}

