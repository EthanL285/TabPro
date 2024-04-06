#include "loginui.h"

loginUI::loginUI(MainWindow *parent) : mainWindow(parent)
{
    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter);
    layout->setSpacing(20);
    layout->setContentsMargins(0, 10, 0, 10);                               // left, top, right, bottom

    // Load icon
    QPixmap appIcon(":/AppIcon/Icon1.png");
    QSize iconSize(100, 100);
    appIcon = appIcon.scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Create QLabel to display icon
    QLabel *iconLabel = new QLabel(this);
    iconLabel->setPixmap(appIcon);
    iconLabel->setFixedSize(iconSize);

    // Add icon to layout
    layout->addWidget(iconLabel);
    layout->setAlignment(iconLabel, Qt::AlignHCenter | Qt::AlignTop);

    // Create login box
    RectangleWidget *loginBox = new RectangleWidget(this);
    layout->addWidget(loginBox);

    // Create text layout
    QVBoxLayout *textLayout = new QVBoxLayout(loginBox);
    textLayout->setAlignment(Qt::AlignHCenter);
    textLayout->setContentsMargins(40, 20, 40, 10);
    textLayout->setSpacing(30);

    // Add title and decorative lines
    QHBoxLayout *titleLayout = new QHBoxLayout();
    textLayout->addLayout(titleLayout);

    QFrame *lineFrameLeft = new QFrame();
    lineFrameLeft->setFrameShape(QFrame::HLine);
    lineFrameLeft->setFrameShadow(QFrame::Sunken);
    titleLayout->addWidget(lineFrameLeft);

    QLabel *title = new QLabel("TabPro");
    title->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    title->setStyleSheet("color: white; font: 30pt Moon;");
    titleLayout->addWidget(title);

    QFrame *lineFrameRight = new QFrame();
    lineFrameRight->setFrameShape(QFrame::HLine);
    lineFrameRight->setFrameShadow(QFrame::Sunken);
    titleLayout->addWidget(lineFrameRight);

    // Add phrase
    QLabel *phrase = new QLabel("Harmonise Your Music");
    phrase->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    phrase->setStyleSheet("color: white; font: 12pt Moon;");
    textLayout->addWidget(phrase);
    textLayout->setAlignment(phrase, Qt::AlignHCenter);

    // Create text fields
    QLineEdit *username = new QLineEdit(loginBox);
    QLineEdit *password = new QLineEdit(loginBox);

    username->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    password->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    username->setPlaceholderText("Username");
    password->setPlaceholderText("Password");

    textLayout->addWidget(username);
    textLayout->addWidget(password);

    username->setMinimumSize(0, 50);
    password->setMinimumSize(0, 50);

    username->setStyleSheet
    (
        "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(96, 94, 92, 200), stop: 1 rgba(32, 31, 30, 200));"
        "border: none;"
        "color: white;"
        "font: 12pt Muli;"
        "padding-left: 6px;"
    );

    password->setStyleSheet
    (
        "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(96, 94, 92, 200), stop: 1 rgba(32, 31, 30, 200));"
        "border: none;"
        "color: white;"
        "font: 12pt Muli;"
        "padding-left: 6px;"
    );

    // Add icons to text fields
    QHBoxLayout *userLayout = new QHBoxLayout();
    QHBoxLayout *passwordLayout = new QHBoxLayout();

    textLayout->addLayout(userLayout);
    textLayout->addLayout(passwordLayout);
    userLayout->setSpacing(0);
    passwordLayout->setSpacing(0);

    QPixmap userIcon(":/FieldIcon/user.png");
    QPixmap passwordIcon(":/FieldIcon/lock.png");

    QSize fieldIcon(50, 50);
    userIcon = userIcon.scaled(fieldIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    passwordIcon = passwordIcon.scaled(fieldIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QLabel *userLabel = new QLabel();
    QLabel *passwordLabel = new QLabel();

    userLabel->setStyleSheet("background-color: black;");
    passwordLabel->setStyleSheet("background-color: black;");

    userLabel->setPixmap(userIcon);
    passwordLabel->setPixmap(passwordIcon);

    userLayout->addWidget(userLabel);
    passwordLayout->addWidget(passwordLabel);

    userLayout->addWidget(username);
    passwordLayout->addWidget(password);

    // Add 'remember me' and 'forgot password'
    QHBoxLayout *accountLayout = new QHBoxLayout();
    textLayout->addLayout(accountLayout);

    QCheckBox *rememberMe = new QCheckBox("Remember me");
    accountLayout->addWidget(rememberMe);
    rememberMe->setStyleSheet ("color: gray; font: 11pt Muli;");
    connect(rememberMe, &QCheckBox::stateChanged, this, &loginUI::rememberSlot);

    QLabel *forgotPassword = new ClickableLabel("Forgot password?");
    forgotPassword->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    forgotPassword->setStyleSheet("color: gray; font: Italic 11pt Muli;");
    accountLayout->addWidget(forgotPassword);

    // Add login button
    QPushButton *loginButton = new QPushButton("Login", this);
    loginButton->setMinimumSize(0, 50);
    connect(loginButton, &QPushButton::clicked, this, &loginUI::loginSlot);  // Connect signal (QPushButton::clicked) to login slot

    loginButton->setStyleSheet
     (
       "QPushButton {"
           "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(255, 255, 255, 200), stop: 1 rgba(96, 94, 92, 200));"
           "border: none;"
           "color: black;"
           "font: 15pt Moon;"
       "}"
       "QPushButton:hover {"
           "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(255, 255, 255, 200), stop: 0.2 rgba(243, 242, 241, 200));"
       "}"
       "QPushButton:pressed {"
           "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba(32, 31, 30, 200), stop: 0.3 rgba(59, 58, 57, 200));" // Vertical gradient
           "color: white;"
           "font: 15pt Moon;"
       "}"
    );

    loginButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    textLayout->addWidget(loginButton);
}


// loginUI Slots
void loginUI::loginSlot()
{
    qDebug() << "Login Successful";
}

void loginUI::rememberSlot()
{
    qDebug() << "Tick Successful";
}

// ClickableLabel Class
ClickableLabel::ClickableLabel(const QString &text, QWidget *parent) : QLabel(text, parent)
{
    setStyleSheet("color: gray; font: Italic 11pt Muli;");
}

void ClickableLabel::forgotSlot()
{
    qDebug() << "Link Successful";
}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        ClickableLabel::forgotSlot();
    }
}

void ClickableLabel::enterEvent(QEnterEvent *event)
{
    QLabel::enterEvent(event);
    setStyleSheet("color: white; font: Italic 11pt Muli;");
}

void ClickableLabel::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    setStyleSheet("color: gray; font: Italic 11pt Muli;");
}

// RectangleWidget Class
RectangleWidget::RectangleWidget(QWidget *parent) {
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setAttribute(Qt::WA_TranslucentBackground);
}

void RectangleWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Create gradient with transparent colours
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, QColor(32, 31, 30, 240));
    gradient.setColorAt(0.5, QColor(0, 0, 0, 240));
    gradient.setColorAt(1, QColor(32, 31, 30, 240));

    // Draw rounded rectangle with gradient
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(gradient));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 15, 15);
}

void RectangleWidget::resizeEvent(QResizeEvent *event)
{
    // Calculate new rectangle size (2:3 aspect ratio)
    int newHeight = this->height();                                         // Height due to expanding size policy
    int newWidth = static_cast<int>(newHeight * aspectRatio);

    // Calculate new centre position of rectangle
    int x = this->pos().x() + (this->width() - newWidth) / 2;
    int y = this->pos().y() + (this->height() - newHeight) / 2;

    // Resize rectangle from center position
    this->setGeometry(x, y, newWidth, newHeight);
}

QSize RectangleWidget::sizeHint() const
{
    // Set initial size of rectangle
    int height = 561;
    int width = static_cast<int>(height * aspectRatio);

    return QSize(width, height);
}
