#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "loginui.h"
#include "registerui.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set app icon
    QIcon appIcon(":/AppIcon/Icon1.png");
    this->setWindowIcon(appIcon);

    // Set initial and minimum window size
    this->resize(1600, 900);
    this->setMinimumSize(1600, 900);

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

    // Create loginUI object
    loginBox = new loginUI(this);
    stackedWidget->addWidget(loginBox);

    // Create registerUI object
    registerBox = new RegisterUI(this);
    stackedWidget->addWidget(registerBox);
}

// MainWindow slots
void MainWindow::redirectLogin()
{
    stackedWidget->setCurrentWidget(loginBox);
}

void MainWindow::redirectRegister()
{
    stackedWidget->setCurrentWidget(registerBox);
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
