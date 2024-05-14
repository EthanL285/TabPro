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

    QPushButton *loginButton = new MainButton("Sign Up", registerBox);
    widgetLayout->addWidget(loginButton);
    connect(loginButton, &QPushButton::clicked, this, &RegisterUI::registerSlot);

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
}

// RegisterUI Slot
void RegisterUI::registerSlot()
{
    // Retrieve QLineEdit from widget fields
    QLineEdit *email = this->email->findChild<QLineEdit*>("field");
    QLineEdit *username = this->username->findChild<QLineEdit*>("field");
    QLineEdit *password = this->password->findChild<QLineEdit*>("field");

    // Check if fields are empty
    bool fieldEmpty = false;

    if (email->text() == "")
    {
        setRedBorder(this->email);
        fieldEmpty = true;
    }
    if (username->text() == "")
    {
        setRedBorder(this->username);
        fieldEmpty = true;
    }
    if (password->text() == "")
    {
        setRedBorder(this->password);
        fieldEmpty = true;
    }

    // If at least one field is empty
    if (fieldEmpty)
    {
        // Create new layout containing password field, incomplete text and sign-in button
        QVBoxLayout *textLayout = new QVBoxLayout();
        QWidget *registerButton = widgetLayout->itemAt(7)->widget();
        QWidget *passwordField = widgetLayout->itemAt(5)->widget();
        QSpacerItem *verticalSpacer = static_cast<QSpacerItem*>(widgetLayout->itemAt(6));

        QLabel *incompleteText = new QLabel(QString::fromUtf8("\u2717 ") + "Incomplete Fields");
        incompleteText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        incompleteText->setStyleSheet("color: rgb(237, 67, 55); font: 11pt Muli;");
        incompleteText->setContentsMargins(0, 20, 0, 20);

        textLayout->addWidget(passwordField);
        textLayout->addWidget(incompleteText);
        textLayout->addWidget(registerButton);
        textLayout->setContentsMargins(0, 0, 0, 0);
        textLayout->setSpacing(0);

        // Replace widgets with new layout
        widgetLayout->removeWidget(registerButton);
        widgetLayout->removeWidget(passwordField);
        widgetLayout->removeItem(verticalSpacer);
        widgetLayout->insertLayout(5, textLayout);
        widgetLayout->setContentsMargins(40, 30, 40, 71);
    }
    // If all fields are filled
    else
    {
        // Add user to database
        usermodel->addUser(email->text(), username->text());

        // Add success text
        if (registerSuccess == nullptr)
        {
            registerSuccess = new QLabel(QString::fromUtf8("\u2713 ") + "Account successfully created");
            registerSuccess->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            registerSuccess->setStyleSheet("color: rgb(114, 191, 106); font: 11pt Muli;");
            widgetLayout->setContentsMargins(40, 30, 40, 31);
            widgetLayout->addWidget(registerSuccess);
            widgetLayout->setAlignment(registerSuccess, Qt::AlignHCenter);
        }
    }
}

// Delete register success text
void RegisterUI::removeText()
{
    if (registerSuccess != nullptr)
    {
        widgetLayout->removeWidget(registerSuccess);
        widgetLayout->setContentsMargins(40, 30, 40, 80);

        // Delete text once fade transition ends
        QTimer::singleShot(500, this, [this](){
            delete registerSuccess;
            registerSuccess = nullptr;
        });
    }
}

// Set widget border to red
void RegisterUI::setRedBorder(QWidget *fieldWidget)
{
    // Retrieve field and icon from widget
    QLineEdit *field = fieldWidget->findChild<QLineEdit*>("field");
    QLabel *fieldIcon = fieldWidget->findChild<QLabel*>("icon");

    // Set field and icon border to red
    field->setStyleSheet
    (
        "#" + field->objectName() +   // Change parent widget only (for fields with labels)
        " {"
        "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(96, 94, 92, 100), stop: 1 rgba(32, 31, 30, 200));"
        "border-top: 0.5px solid rgb(237, 67, 55);"
        "border-right: 0.5px solid rgb(237, 67, 55);"
        "border-bottom: 0.5px solid rgb(237, 67, 55);"
        "border-left: none;"
        "color: white;"
        "font: 12pt Muli;"
        "padding-left: 6px;"
        "}"
    );
    fieldIcon->setStyleSheet
    (
        "background-color: rgba(0, 0, 0, 100);"
        "border-top: 0.5px solid rgb(237, 67, 55);"
        "border-right: none;"
        "border-bottom: 0.5px solid rgb(237, 67, 55);"
        "border-left: 0.5px solid rgb(237, 67, 55);"
    );
}

// ClickableLabel Class
void ClickableLabel::backToLogin()
{
    mainWindow->redirectLogin();
}

