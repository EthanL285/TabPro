#include "loginui.h"

loginUI::loginUI(MainWindow *parent)
    : mainWindow(parent)
{
    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter);
    layout->setSpacing(20);
    layout->setContentsMargins(0, 10, 0, 10);   // left, top, right, bottom

    // Load icon
    QPixmap appIcon(":/AppIcon/Icon1.png");
    QSize iconSize(100, 100);
    appIcon = appIcon.scaled(iconSize, Qt::KeepAspectRatio);

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
}

RectangleWidget::RectangleWidget(QWidget *parent) {
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setAttribute(Qt::WA_TranslucentBackground);
}

void RectangleWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // Set antialiasing for smooth edges
    painter.setBrush(QColor(32, 31, 30, 240));
    painter.drawRect(rect());                       // Dimensions of rectangle widget
}

void RectangleWidget::resizeEvent(QResizeEvent *event)
{
    // Calculate new rectangle size (2:3 aspect ratio)
    const double aspectRatio = 2.0 / 3.0;
    int newHeight = this->height();
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
    const double aspectRatio = 2.0 / 3.0;
    int height = 561;
    int width = static_cast<int>(height * aspectRatio);

    return QSize(width, height);
}
