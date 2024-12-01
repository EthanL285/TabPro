#include "mainwindow.h"
#include "loginui.h"
#include "registerui.h"
#include "transitions.h"
#include "passwordui.h"
#include "mainwidget.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create UserModel object for database-related operations
    usermodel = new UserModel(this);

    // Set app icon
    QIcon appIcon(":/app/app/AppIcon.png");
    this->setWindowIcon(appIcon);

    // Set initial and minimum window size
    this->resize(1440, 900);
    this->setMinimumSize(1440, 900);

    // Load background image
    backgroundImage = QPixmap(":/background/background/Background Grayscale.png");

    // Set background image to intial window size
    QPixmap scaledBgImage = backgroundImage;
    scaledBgImage = scaledBgImage.scaled(this->size(), Qt::KeepAspectRatio);

    // Create QLabel to display background image
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(scaledBgImage);
    backgroundLabel->setGeometry(0, 0, this->size().width(), this->size().height());

    // Create central and stacked widget to manage pages
    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    stackedWidget = new QStackedWidget(centralWidget);

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(stackedWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create loginUI and Transitions object
    loginBox = new loginUI(this, usermodel);
    transition = new Transitions(this);
    stackedWidget->addWidget(loginBox);

    // Automatically login user if token exists
    if (usermodel->tokenExists())
    {
        qDebug() << "Token exists. Logging in";
        redirectMainWidget();
        // UserCredentials usercredentials = usermodel->getUserCredentials();
    }
}

// MainWindow slots
void MainWindow::redirectLogin()
{
    // Add transition and change widgets
    int index = stackedWidget->indexOf(loginBox);
    transition->fadeInOut(stackedWidget->currentWidget(), stackedWidget->widget(index), stackedWidget);

    // Remove error messages of existing pages
    if (registerBox != nullptr)
    {
        RegisterUI *registerUI = static_cast<RegisterUI*>(registerBox);
        registerUI->removeText();
        registerUI->removeErrorMessage(500);
    }
    if (passwordBox == nullptr) return;

    PasswordUI *passwordUI = static_cast<PasswordUI*>(passwordBox);
    int pageNumber = passwordUI->getPageNumber();

    // Remove error messages and handle specific page actions
    passwordUI->removeErrorMessage(500, pageNumber == 0 ? 172 : pageNumber == 1 ? 144 : 160);

    // Verification page
    if (pageNumber == 0)
    {
        usermodel->disconnectFromSMTPServer(); // Disconnect from socket if active still
        return;
    }
    // Code input or reset password page
    QTimer::singleShot(500, this, [this, pageNumber, passwordUI]()
    {
       if (pageNumber == 1) passwordUI->removeVerificationPage();
       else passwordUI->removeResetPasswordPage();
    });
}

void MainWindow::redirectRegister()
{
    // Create registerUI object if first time
    if (registerBox == nullptr)
    {
        registerBox = new RegisterUI(this, usermodel);
        stackedWidget->addWidget(registerBox);
    }
    // Add transition and change widgets
    int index = stackedWidget->indexOf(registerBox);
    transition->fadeInOut(stackedWidget->currentWidget(), stackedWidget->widget(index), stackedWidget);

    // Remove login error message
    static_cast<loginUI*>(loginBox)->removeErrorMessage(500);
}

void MainWindow::redirectPassword()
{
    // Create passwordUI object if first time
    if (passwordBox == nullptr)
    {
        passwordBox = new PasswordUI(this, usermodel);
        stackedWidget->addWidget(passwordBox);
    }
    // Add transition and change widgets
    int index = stackedWidget->indexOf(passwordBox);
    transition->fadeInOut(stackedWidget->currentWidget(), stackedWidget->widget(index), stackedWidget);

    // Remove login error message
    static_cast<loginUI*>(loginBox)->removeErrorMessage(500);
}

void MainWindow::redirectMainWidget()
{
    if (mainWidget != nullptr) return;

    // Create MainWidget object
    mainWidget = new MainWidget(this);
    stackedWidget->addWidget(mainWidget);
    stackedWidget->setCurrentWidget(mainWidget);

    QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect();
    blurEffect->setBlurRadius(80);
    backgroundLabel->setGraphicsEffect(blurEffect);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    // Resize background image to fit window size
    QPixmap scaledBgImage = backgroundImage.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    // Calculate new centre position
    int x = (this->width() - scaledBgImage.width()) / 2;
    int y = (this->height() - scaledBgImage.height()) / 2;

    // Resize background image from centre position
    backgroundLabel->setPixmap(scaledBgImage);
    backgroundLabel->setGeometry(x, y, scaledBgImage.size().width(), scaledBgImage.size().height());
}
