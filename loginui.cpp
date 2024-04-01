#include "loginui.h"

loginUI::loginUI(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

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

    // Set icon alignment
    layout->setAlignment(iconLabel, Qt::AlignHCenter | Qt::AlignTop);

    // Create login box
    RectangleWidget *loginBox = new RectangleWidget(this);
    layout->addWidget(loginBox);
    layout->setAlignment(loginBox, Qt::AlignHCenter);
}

RectangleWidget::RectangleWidget(QWidget *parent) {
    this->setFixedSize(600, 700);
}

void RectangleWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(QColor(32, 31, 30));
    painter.drawRect(rect()); // Dimensions of rectangle widget
}
