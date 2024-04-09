#include "registerui.h"
#include "loginui.h"

RegisterUI::RegisterUI(MainWindow *parent) : mainWindow(parent)
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

    // Add title
    QLabel *title = new QLabel("Create Account");
    title->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    title->setStyleSheet("color: white; font: 30pt Moon;");
    widgetLayout->addWidget(title);
    widgetLayout->setAlignment(title, Qt::AlignHCenter);

    // Create text fields
    QWidget *email = new TextField("Email", ":/FieldIcon/email.png", registerBox);
    QWidget *username = new TextField("Username", ":/FieldIcon/user.png", registerBox);
    QWidget *password = new TextField("Password", ":/FieldIcon/lock.png", registerBox);

    widgetLayout->addWidget(email);
    widgetLayout->addWidget(username);
    widgetLayout->addWidget(password);

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

// ClickableLabel Class
void ClickableLabel::backToLogin()
{
    mainWindow->redirectLogin();
}

