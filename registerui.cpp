#include "registerui.h"
#include "uiwidgets.h"
#include "loginui.h"

RegisterUI::RegisterUI(MainWindow *parent) : QWidget(parent), mainWindow(parent)
{
    // Create UserModel object to add users to database
    usermodel = new UserModel;

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
    QPixmap appIcon(":/AppIcon/IconStripped.png");
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
    email = new TextField("Email", ":/FieldIcon/email.png", registerBox);
    username = new TextField("Username", ":/FieldIcon/user.png", registerBox);
    password = new TextField("Password", ":/FieldIcon/lock.png", registerBox);

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
    fieldEmpty = emptyFieldCheck(this->password, password) || fieldEmpty;
    fieldEmpty = emptyFieldCheck(this->username, username) || fieldEmpty;
    fieldEmpty = emptyFieldCheck(this->email, email) || fieldEmpty;

    // At least one field is empty
    if (fieldEmpty)
    {
        addErrorMessage(QString::fromUtf8("\u2717 ") + "Please fill in all required fields");
    }

    // All fields are filled
    else
    {
        // Validate email
        QString emailMessage = usermodel->isValidEmail(email->text());
        QString passwordMessage = usermodel->isValidPassword(password->text());
        QString usernameMessage = usermodel->isValidUsername(username->text());

        // Invalid input
        if (emailMessage != "Valid")
        {
            invalidInput(this->email, emailMessage); // Add error message and red border
            email->setFocus();
        }
        else if (usernameMessage != "Valid")
        {
            invalidInput(this->username, usernameMessage);
            username->setFocus();
        }
        else if (passwordMessage != "Valid")
        {
            invalidInput(this->password, passwordMessage);
            password->setFocus();
        }
        // Valid Input
        else
        {
            // Add user to database
            usermodel->addUser(email->text(), username->text());

            // Remove error message if invalid attempt prior
            removeErrorMessage(0);

            // If first time registering
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
    }
}

// Creates error message for invalid inputs
void RegisterUI::invalidInput(QWidget *fieldParent, QString &message)
{
    // Invalid input
    if (message != "Valid")
    {
        // Set error message and border
        setRedBorder(fieldParent, true);
        addErrorMessage(message);
    }
}

// Checks if fields are empty
bool RegisterUI::emptyFieldCheck(QWidget *fieldParent, QLineEdit *field)
{
    if (field->text().isEmpty())
    {
        setRedBorder(fieldParent, true);
        field->setFocus();
        return true;
    }
    else
    {
        setRedBorder(fieldParent, false);
        return false;
    }
}

// Create new layout containing password field, failure text and sign-in button
void RegisterUI::addErrorMessage(const QString &message)
{
    // Error message does not exist
    if (errorMessage == nullptr)
    {
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
    // Error message already exists
    else
    {
        errorMessage->setText(message);
    }
}

// Delete register success text
void RegisterUI::removeText()
{
    if (registerSuccess != nullptr)
    {
        // Stop any active timer
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
}

// Delete error message after 'wait' ms
void RegisterUI::removeErrorMessage(int wait)
{
    // Error message exists and function is not currently processing
    if (errorMessage != nullptr && errorLayout != nullptr)
    {
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

            setRedBorder(this->email, false);
            setRedBorder(this->username, false);
            setRedBorder(this->password, false);

            // Restore to original layout
            widgetLayout->insertWidget(5, passwordField);
            widgetLayout->insertItem(6, registerSpacer);
            widgetLayout->insertWidget(7, registerButton);

            delete errorMessage;
            delete errorLayout;
            errorMessage = nullptr;
            errorLayout = nullptr;

            delete errorMessageTimer; // Delete the timer object
            errorMessageTimer = nullptr; // Reset the timer pointer
        });

        // Start the timer
        errorMessageTimer->start(wait);
    }
}

// Set widget border to red
void RegisterUI::setRedBorder(QWidget *fieldWidget, bool setRed)
{
    // Retrieve field and icon from widget
    QLineEdit *field = fieldWidget->findChild<QLineEdit*>("field");
    QLabel *fieldIcon = fieldWidget->findChild<QLabel*>("icon");

    // Set border colour
    QString borderColour;
    if (setRed)
    {
        borderColour = "0.5px solid rgb(237, 67, 55)";
    }
    else
    {
        borderColour = "none";
    }

    // Set field and icon border to red
    field->setStyleSheet
    (
        "#" + field->objectName() +   // Change parent widget only (for fields with labels)
        " {"
        "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(96, 94, 92, 100), stop: 1 rgba(32, 31, 30, 200));"
        "border-top: " + borderColour + ";"
        "border-right: " + borderColour + ";"
        "border-bottom: " + borderColour + ";"
        "border-left: none;"
        "color: white;"
        "font: 12pt Muli;"
        "padding-left: 6px;"
        "}"
    );
    fieldIcon->setStyleSheet
    (
        "background-color: rgba(0, 0, 0, 100);"
        "border-top: " + borderColour + ";"
        "border-right: none;"
        "border-bottom: " + borderColour + ";"
        "border-left: " + borderColour + ";"
    );
}

// ClickableLabel Class
void ClickableLabel::backToLogin()
{
    mainWindow->redirectLogin();
}

