#include "passwordui.h"
#include "loginui.h"

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
    QLabel *info = new QLabel("Enter your account email address and we will share a 4 digit<br>verification code to reset your password");
    info->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
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

void PasswordUI::sendResetEmail()
{
    qDebug() << "Email Sent";
}
