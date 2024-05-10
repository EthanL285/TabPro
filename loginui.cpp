#include "loginui.h"

loginUI::loginUI(MainWindow *parent) : mainWindow(parent)
{
    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter);
    layout->setSpacing(20);
    layout->setContentsMargins(0, 70, 0, 50);                               // left, top, right, bottom

    // Create login box
    RectangleWidget *loginBox = new RectangleWidget(this);
    layout->addWidget(loginBox);

    // Create text layout
    QVBoxLayout *textLayout = new QVBoxLayout(loginBox);
    textLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    textLayout->setContentsMargins(40, 50, 40, 70);
    textLayout->setSpacing(30);

    // Add icon
    QPixmap appIcon(":/AppIcon/IconStripped.png");
    QLabel *iconLabel = new ResizableImageLabel(appIcon, this);

    iconLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    iconLabel->setMaximumSize(234, 234);

    textLayout->addWidget(iconLabel);
    textLayout->setAlignment(iconLabel, Qt::AlignHCenter);

    QSpacerItem *verticalSpacer = new QSpacerItem(0, 10, QSizePolicy::Fixed, QSizePolicy::Fixed);
    textLayout->addItem(verticalSpacer);

    // Add title and decorative line
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
    QWidget *username = new TextField("Username", ":/FieldIcon/user.png", loginBox);
    QWidget *password = new TextField("Password", ":/FieldIcon/lock.png", loginBox);
    username->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    password->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    textLayout->addWidget(username);
    textLayout->addWidget(password);

    // Add 'remember me' and 'forgot password'
    QHBoxLayout *accountLayout = new QHBoxLayout();
    textLayout->addLayout(accountLayout);

    QCheckBox *rememberMe = new QCheckBox("Remember me");
    accountLayout->addWidget(rememberMe);
    rememberMe->setStyleSheet ("color: gray; font: 11pt Muli;");
    connect(rememberMe, &QCheckBox::stateChanged, this, &loginUI::rememberSlot);

    QLabel *forgotPassword = new ClickableLabel("Forgot password?", mainWindow);
    forgotPassword->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    forgotPassword->setStyleSheet("color: gray; font: Italic 11pt Muli;");
    forgotPassword->setCursor(Qt::PointingHandCursor);
    accountLayout->addWidget(forgotPassword);

    // Add login button
    QPushButton *loginButton = new MainButton("Login");
    textLayout->addWidget(loginButton);
    connect(loginButton, &QPushButton::clicked, this, &loginUI::loginSlot);  // Connect signal (QPushButton::clicked) to login slot

    // Add register
    QHBoxLayout *registerLayout = new QHBoxLayout();
    textLayout->addLayout(registerLayout);
    registerLayout->setAlignment(Qt::AlignHCenter);
    registerLayout->setSpacing(0);

    QLabel *signup = new QLabel("Don't have an account?  ");
    signup->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    signup->setStyleSheet("color: gray; font: Italic 11pt Muli;");
    registerLayout->addWidget(signup);

    QLabel *reg = new ClickableLabel("Register", mainWindow);
    reg->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    reg->setStyleSheet("color: white; font: 11pt Muli;");
    reg->setCursor(Qt::PointingHandCursor);
    registerLayout->addWidget(reg);
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

// ResizeableImageLabel Class
ResizableImageLabel::ResizableImageLabel(const QPixmap image, QWidget* parent) : image(image), QLabel(parent) {}

void ResizableImageLabel::resizeEvent(QResizeEvent* event)
{
    QLabel::resizeEvent(event);

    // Scale image to fit label
    QPixmap scaledImage = image.scaled(QSize(this->height(), this->height()), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    this->setPixmap(scaledImage);
}

// ClickableLabel Class
ClickableLabel::ClickableLabel(const QString &text, MainWindow *mainWindow) : QLabel(text, mainWindow), mainWindow(mainWindow)
{
    setStyleSheet("color: gray; font: Italic 11pt Muli;");
}

void ClickableLabel::forgotSlot()
{
    qDebug() << "Forgot Password Successful";
}

void ClickableLabel::registerAccount()
{
    mainWindow->redirectRegister();
}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    if (text() == "Forgot password?")
    {
        ClickableLabel::forgotSlot();
    }
    else if (text() == "Register")
    {
        ClickableLabel::registerAccount();
    }
    else if (text() == "Login")
    {
        ClickableLabel::backToLogin();
    }
}

void ClickableLabel::enterEvent(QEnterEvent *event)
{
    QLabel::enterEvent(event);
    if (text() == "Forgot password?")
    {
        setStyleSheet("color: white; font: Italic 11pt Muli;");
    }
    else if (text() == "Register" || "Login")
    {
        setStyleSheet("color: white; font: Italic 11pt Muli; text-decoration: underline;");
    }
}

void ClickableLabel::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    if (text() == "Forgot password?")
    {
        setStyleSheet("color: gray; font: Italic 11pt Muli;");
    }
    else if (text() == "Register" || "Login")
    {
        setStyleSheet("color: white; font: Italic 11pt Muli;");
    }
}

// RectangleWidget Class
RectangleWidget::RectangleWidget(QWidget *parent)
{
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
