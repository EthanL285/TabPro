#include "loginui.h"

loginUI::loginUI(MainWindow *parent, UserModel *usermodel) : QWidget(parent), mainWindow(parent), usermodel(usermodel)
{
    setContentsMargins(0, 0, 0, 22);

    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter);
    layout->setSpacing(20);
    layout->setContentsMargins(0, 70, 0, 50);

    // Create login box
    RectangleWidget *loginBox = new RectangleWidget(this);
    layout->addWidget(loginBox);

    // Create text layout
    textLayout = new QVBoxLayout(loginBox);
    textLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    textLayout->setContentsMargins(40, 30, 40, 70);
    textLayout->setSpacing(30);

    // Add icon
    QPixmap appIcon(":/app/app/AppIconStripped.png");
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
    emailParent = new TextField("Email", ":/field/field/User.png", loginBox);
    passwordParent = new TextField("Password", ":/field/field/Lock.png", loginBox);
    emailParent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    passwordParent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    textLayout->addWidget(emailParent);
    textLayout->addWidget(passwordParent);

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

    // Connect the returnPressed() signal of text fields to the click() signal of button
    QLineEdit *emailField = emailParent->findChild<QLineEdit*>("field");
    QLineEdit *passwordField = passwordParent->findChild<QLineEdit*>("field");

    connect(emailField, &QLineEdit::returnPressed, loginButton, &QPushButton::click);
    connect(passwordField, &QLineEdit::returnPressed, loginButton, &QPushButton::click);
}

// loginUI Slots
void loginUI::loginSlot()
{
    // Retrieve fields from parent widgets
    QLineEdit *emailField = this->emailParent->findChild<QLineEdit*>("field");
    QLineEdit *passwordField = this->passwordParent->findChild<QLineEdit*>("field");

    // Check for empty fields
    bool fieldEmpty = false;

    fieldEmpty = this->passwordParent->emptyFieldCheck(passwordField) || fieldEmpty;
    fieldEmpty = this->emailParent->emptyFieldCheck(emailField) || fieldEmpty;

    // At least one field is empty
    if (fieldEmpty)
    {
         addErrorMessage(QString::fromUtf8("\u2717 ") + "Please fill in all required fields");
    }
    // All fields are filled
    else
    {
        // Verify user
        QString message = usermodel->verifyUser(emailParent, passwordParent); // Also sets border and focus

        // Invalid input
        if (message != "Valid")
        {
            // Add error message
            addErrorMessage(message);
        }
        // Valid Input
        else
        {
            // Remember user credentials
            if (remember)
            {
                usermodel->rememberUserCredentials(emailField->text());
            }
            // Forget user credentials if email has existing token
            else
            {
                // Remove user's existing token
                if (usermodel->tokenExists(emailField->text()))
                {
                    usermodel->removeToken();
                }
            }
            // Remove error message if invalid attempt prior
            removeErrorMessage(0);

            // Clear text from field
            emailField->clear();
            passwordField->clear();

            // Redirect to main widget
            mainWindow->redirectMainWidget();
        }
    }
}

// Remove error message from UI
void loginUI::removeErrorMessage(int wait)
{
    // Error message exists and function is not currently processing
    if (errorMessage != nullptr && errorLayout != nullptr)
    {
        if (errorMessageTimer && errorMessageTimer->isActive())
        {
            errorMessageTimer->stop();
        }
        // Create a new timer
        errorMessageTimer = new QTimer(this);
        errorMessageTimer->setSingleShot(true);

        // Remove error message from widget once transition ends (timer expires)
        connect(errorMessageTimer, &QTimer::timeout, this, [this]() {
            QWidget *passwordField = errorLayout->itemAt(0)->widget();
            QLayout *layout = errorLayout->itemAt(2)->layout();

            errorLayout->removeWidget(passwordField);
            errorLayout->removeWidget(errorMessage);
            errorLayout->removeItem(layout);
            textLayout->removeItem(errorLayout);

            emailParent->setRedBorder(false);
            passwordParent->setRedBorder(false);

            // Restore to original layout
            textLayout->insertWidget(5, passwordField);
            textLayout->insertItem(6, layout);
            textLayout->setContentsMargins(40, 30, 40, 70);

            delete errorMessage;
            delete errorLayout;
            errorMessage = nullptr;
            errorLayout = nullptr;

            delete errorMessageTimer; // Delete the timer object
            errorMessageTimer = nullptr; // Reset the timer pointer
        });
        // Start the timer
        errorMessageTimer->start(wait);
    }
}

// Add error message to UI
void loginUI::addErrorMessage(const QString &message)
{
    // Error message does not exist
    if (errorMessage == nullptr)
    {
        // Create and add new layout with error message
        errorLayout = new QVBoxLayout();
        QWidget *passwordField = textLayout->itemAt(5)->widget();
        QLayout *layout = textLayout->itemAt(6)->layout();

        errorMessage = new QLabel(message);
        errorMessage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        errorMessage->setStyleSheet("color: rgb(237, 67, 55); font: 11pt Muli;");

        textLayout->removeItem(layout);
        textLayout->removeWidget(passwordField);

        errorLayout->addWidget(passwordField);
        errorLayout->addWidget(errorMessage);
        errorLayout->addLayout(layout);

        errorLayout->setContentsMargins(0, 0, 0, 0);
        errorLayout->setSpacing(20);

        // Replace widgets with new layout
        textLayout->insertLayout(5, errorLayout);
        textLayout->setContentsMargins(40, 30, 40, 51);
    }
    // Error message already exists
    else
    {
        errorMessage->setText(message);
    }
}

void loginUI::rememberSlot()
{
    remember = !remember;
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
    mainWindow->redirectPassword();
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
