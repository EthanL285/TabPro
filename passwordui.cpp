#include "passwordui.h"
#include "loginui.h"
#include <QTime>

PasswordUI::PasswordUI(MainWindow *parent, UserModel *usermodel) : QWidget(parent), mainWindow(parent), usermodel(usermodel)
{      
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
    emailParent = new TextField("Email", ":/FieldIcon/email.png", passwordBox);
    emailParent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    widgetLayout->addWidget(emailParent);

    // Add send button
    QSpacerItem *verticalSpacer2 = new QSpacerItem(0, 10, QSizePolicy::Fixed, QSizePolicy::Fixed);
    widgetLayout->addItem(verticalSpacer2);

    QPushButton *sendButton = new MainButton("Send", passwordBox);
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

    // Connect the returnPressed() signal of text fields to the click() signal of button
    QLineEdit *email = emailParent->findChild<QLineEdit*>("field");
    connect(email, &QLineEdit::returnPressed, sendButton, &QPushButton::click);
}

// Send reset email to user
void PasswordUI::sendResetEmail()
{
    // Retrieve email field from parent widget
    email = emailParent->findChild<QLineEdit*>("field");

    // Check for empty fields
    bool fieldEmpty = false;
    fieldEmpty = emailParent->emptyFieldCheck(email) || fieldEmpty;

    // Empty email field
    if (fieldEmpty)
    {
        addErrorMessage(QString::fromUtf8("\u2717 ") + "Please fill in all required fields");
    }
    // Filled email field
    else
    {
        // Verify user
        QString message = usermodel->verifyUser(emailParent); // Also sets border and focus

        // Email not found in database
        if (message != "Valid")
        {
            // Add error message
            addErrorMessage(message);
        }
        // Email is found in database
        else
        {
            // Remove error message if invalid attempt prior
            removeErrorMessage(0);

            // Send verification code to user email
            QString code = generateVerificationCode();
            usermodel->sendVerificationEmail(email->text(), code);

            // Clear text from field
            email->clear();
        }
    }
}

// Email successfully delivered
void PasswordUI::onEmailSentSuccess()
{
    info->setText("A 6 digit verification code has been sent to " + email->text());
}

// Add error message to UI
void PasswordUI::addErrorMessage(const QString &message)
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
        errorMessage->setStyleSheet("color: rgb(237, 67, 55); font: 11pt Muli;");

        widgetLayout->removeWidget(emailField);
        widgetLayout->removeWidget(sendButton);
        widgetLayout->removeItem(verticalSpacer);

        errorLayout->addWidget(emailField);
        errorLayout->addWidget(errorMessage);
        errorLayout->addWidget(sendButton);

        errorLayout->setContentsMargins(0, 0, 0, 0);
        errorLayout->setSpacing(20);

        // Replace widgets with new layout
        widgetLayout->insertLayout(4, errorLayout);
        widgetLayout->setContentsMargins(40, 30, 40, 164);
    }
    // Error message already exists
    else
    {
        errorMessage->setText(message);
    }
}

// Delete error message after 'wait' ms
void PasswordUI::removeErrorMessage(int wait)
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
            widgetLayout->setContentsMargins(40, 30, 40, 172);

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

// Generates a verification code
QString PasswordUI::generateVerificationCode()
{
    // Generate random 6 digit code
    QString code;
    for (int i = 0; i < 6; ++i) {
        code.append(QString::number(QRandomGenerator::global()->bounded(10))); // Generates number between 0-9 and appends it
    }
    return code;
}
