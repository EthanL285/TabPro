#include "registerui.h"
#include "loginui.h"
#include "uiwidgets.h"

RegisterUI::RegisterUI(MainWindow *parent) : QWidget(parent), mainWindow(parent)
{
    // Add rectangle layout
    QVBoxLayout *rectangleLayout = new QVBoxLayout(this);
    rectangleLayout->setAlignment(Qt::AlignHCenter);
    rectangleLayout->setSpacing(20);
    rectangleLayout->setContentsMargins(0, 70, 0, 50);

    // Add register box
    RectangleWidget *registerBox = new RectangleWidget(this);
    rectangleLayout->addWidget(registerBox);

    // Add widget layout
    QVBoxLayout *widgetLayout = new QVBoxLayout(registerBox);
    widgetLayout ->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    widgetLayout ->setContentsMargins(40, 50, 40, 70);
    widgetLayout ->setSpacing(30);

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
    QWidget *email = new TextField("Email", ":/FieldIcon/email.png", registerBox);
    QWidget *username = new TextField("Username", ":/FieldIcon/user.png", registerBox);
    QWidget *password = new TextField("Password", ":/FieldIcon/lock.png", registerBox);

    email->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    username->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    password->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    widgetLayout->addWidget(email);
    widgetLayout->addWidget(username);
    widgetLayout->addWidget(password);

    // Add Register button
    QSpacerItem *verticalSpacer2 = new QSpacerItem(0, 10, QSizePolicy::Fixed, QSizePolicy::Fixed);
    widgetLayout->addItem(verticalSpacer2);

    QPushButton *loginButton = new MainButton("Register", registerBox);
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

// RegisterUI Slots
void RegisterUI::registerSlot()
{
    qDebug() << "Register Successful";
}

// ClickableLabel Class
void ClickableLabel::backToLogin()
{
    mainWindow->redirectLogin();
}

