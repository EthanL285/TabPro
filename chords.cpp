#include "chords.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QPropertyAnimation>
#include <QLabel>
#include <QGraphicsEffect>

Chords::Chords(QWidget *parent)
    : QWidget{parent}
{   
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setAlignment(Qt::AlignLeft);

    QVBoxLayout *boxLayout = new QVBoxLayout();
    boxLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(boxLayout);

    // Add box header
    header = new QWidget();
    header->setFixedHeight(60);
    header->setMaximumWidth(0);
    header->setMinimumWidth(0);
    header->setStyleSheet("background: rgb(23,23,23); border-top: 2px solid rgb(20,20,20);");
    header->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    boxLayout->addWidget(header);

    // Add box
    content = new QWidget();
    content->setMinimumWidth(0);
    content->setStyleSheet("background: rgb(45,45,45); border-left: 2px solid gray;");
    content->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    boxLayout->addWidget(content);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(5);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    shadowEffect->setOffset(0, 4);
    setGraphicsEffect(shadowEffect);

    // Add button
    button = new QPushButton(">>");
    button->setCheckable(true);
    button->setFixedWidth(20);
    button->setCursor(Qt::PointingHandCursor);
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    button->setStyleSheet
    (
        "QPushButton {"
        "background: rgb(17,17,17);"
        "border: none;"
        "color: white;"
        "font: 15pt Moon;"
        "font-weight: semi-bold;"
        "}"
        "QPushButton:hover {"
        "background: rgb(22,22,22);"
        "border: 1px solid rgb(17,17,17);"
        "}"
        "QPushButton:pressed {"
        "background: rgb(70,70,70);"
        "font: 15pt Moon;"
        "}"
    );
    mainLayout->addWidget(button);
    connect(button, &QPushButton::toggled, this, &Chords::toggleContent);

    // Header layout
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setAlignment(Qt::AlignLeft);

    // Chord mode section
    QLabel *modeText = new QLabel();
    modeText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    modeText->setStyleSheet("color: white; font: 11pt Muli; font-weight: semi-bold; border: none;");
    modeText->setText("Chord Mode");
    headerLayout->addWidget(modeText);

    QWidget *toggleSwitch = new ToggleSwitch(this);
    headerLayout->addWidget(toggleSwitch);
}

// Toggles the chord content
void Chords::toggleContent(bool checked)
{
    animateAccordion(header);
    animateAccordion(content);
    contentToggled = !contentToggled;
}

// Animates the accordion
void Chords::animateAccordion(QWidget *widget)
{
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "minimumWidth");
    int endWidth = (contentToggled) ? 0 : 500;

    animation->setStartValue(widget->minimumWidth());
    animation->setEndValue(endWidth);
    animation->setDuration(250);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

//////////////////// Toggle Switch Class ////////////////////

ToggleSwitch::ToggleSwitch(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(60, 40);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("border: none;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(6,0,0,0);
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    handle = new QWidget();
    handle->setFixedSize(24,24);
    handle->setStyleSheet("background: white; border-radius: 12px; border: none;");
    mainLayout->addWidget(handle);
}

// Paint event
void ToggleSwitch::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int rectWidth = 55;
    int rectHeight = 30;
    int x = (width() - rectWidth) / 2;
    int y = (height() - rectHeight) / 2;
    QRect rect(x, y, rectWidth, rectHeight);
    QColor backgroundColor = toggled ? QColor(0, 175, 0) : QColor(45, 45, 45);
    painter.setBrush(QBrush(backgroundColor));
    painter.setPen(QPen(QColor(20,20,20), 1));
    painter.drawRoundedRect(rect, 15,15);

    update();
}

// Mouse press event
void ToggleSwitch::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    animateHandle();
    toggled = !toggled;
}

// Animates the switch handle
void ToggleSwitch::animateHandle()
{
    QPropertyAnimation* animation = new QPropertyAnimation(handle, "pos");
    QPoint startPoint = handle->pos();
    QPoint endPoint = startPoint;

    if (!toggled) endPoint.setX(30);
    else endPoint.setX(6);

    animation->setStartValue(startPoint);
    animation->setEndValue(endPoint);
    animation->setDuration(150);
    animation->setEasingCurve(QEasingCurve::Linear);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}


