#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "loginui.h"
#include "registerui.h"
#include "transitions.h"
#include "passwordui.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create UserModel object for database-related operations
    usermodel = new UserModel;

    // Set app icon
    QIcon appIcon(":/AppIcon/Icon1.png");
    this->setWindowIcon(appIcon);

    // Set initial and minimum window size
    this->resize(1440, 900);
    this->setMinimumSize(1440, 900);

    // Load background image
    backgroundImage = QPixmap(":/Background/background.png");

    // Set background image to intial window size
    QPixmap scaledBgImage = backgroundImage;
    scaledBgImage = scaledBgImage.scaled(this->size(), Qt::KeepAspectRatio);

    // Create QLabel to display background image
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(scaledBgImage);
    backgroundLabel->setGeometry(0, 0, this->size().width(), this->size().height());

    // Set background image to black and white
    QGraphicsColorizeEffect *colorize = new QGraphicsColorizeEffect;
    colorize->setColor(Qt::black);
    backgroundLabel->setGraphicsEffect(colorize);

    // Create central and stacked widget to manage pages
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    stackedWidget = new QStackedWidget(centralWidget);

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(stackedWidget);
    mainLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mainLayout->setContentsMargins(0, 0, 0, 0); // Remove any margins

    // Create loginUI object
    loginBox = new loginUI(this, usermodel);
    stackedWidget->addWidget(loginBox);

    // Create Transitiosn object
    transition = new Transitions(this);

    // Automatically login user if token exists
    if (usermodel->tokenExists())
    {
        qDebug() << "Token exists. Logging in";
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
        static_cast<RegisterUI*>(registerBox)->removeText();   // Convert type from *QWidget (Base) to *RegisterUI (Derived)
        static_cast<RegisterUI*>(registerBox)->removeErrorMessage(500);
    }
    if (passwordBox != nullptr)
    {
        static_cast<PasswordUI*>(passwordBox)->removeErrorMessage(500);
    }
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

MainWindow::~MainWindow()
{
    delete ui;
}

