#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "loginui.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set app icon
    QIcon appIcon(":/AppIcon/Icon1.png");
    this->setWindowIcon(appIcon);

    // Set initial and minimum window size
    this->resize(1280,720);
    this->setMinimumSize(1280, 720);

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

    // Create loginUI object
    loginUI *login = new loginUI(this);
    this->setCentralWidget(login);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    // Resize background image to fit window size
    QPixmap scaledBgImage = backgroundImage;
    scaledBgImage = scaledBgImage.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    backgroundLabel->setPixmap(scaledBgImage);
    backgroundLabel->setGeometry(0, 0, this->size().width(), this->size().height());
}

MainWindow::~MainWindow()
{
    delete ui;
}
