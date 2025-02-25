#include "tabfret.h"

#include <QPainter>
#include <QKeyEvent>

TabFret::TabFret(int string, QWidget *parent)
    : string{string}, QPushButton{parent}
{
    setCheckable(true);
    connect(this, &QPushButton::toggled, this, &TabFret::onToggle);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet
    (
        "QPushButton { border-radius: 1px; background-color: rgb(23,23,23); font: 20pt Consolas; }"
        "QPushButton:hover { background-color: rgb(75,75,75); }"
    );
}

// Getter for string
int TabFret::getString()
{
    return string;
}

// Getter for fret
int TabFret::getFret()
{
    return fret;
}

// Setter for fret
void TabFret::setFret(int fret)
{
    this->fret = fret;
}

// Key Press Event
void TabFret::keyPressEvent(QKeyEvent *event)
{
    if (event->key() < Qt::Key_0 || event->key() > Qt::Key_9) return;
    fret = event->key() - Qt::Key_0;

    update();
    emit keyPressed(fret, string);
}

// Paint event
void TabFret::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);;
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw state
    painter.setPen(Qt::NoPen);
    painter.setBrush(fretStateColours[state]);
    painter.drawRect(0, 0, width(), height());

    // Draw tab line
    painter.setPen(QPen(Qt::white, 1.5));
    painter.drawLine(0, height() / 2, width(), height() / 2);

    // Draw fret number
    if (fret == EMPTY_FRET) return;

    QRect textRect = painter.boundingRect(rect(), Qt::AlignCenter, QString::number(fret));
    int textX = (width() - textRect.width()) / 2;
    int textY = (height() / 2) - (textRect.height() / 2) - 1;

    // Draw text and background
    if (!isChecked() && state != State::Hovered) painter.setBrush(textBackground);
    painter.setFont(QFont("Consolas", 20));
    painter.setPen(QPen(Qt::NoPen));
    painter.drawRect(textX, textY, textRect.width(), textRect.height());
    painter.setPen(QPen(Qt::white, 1.5));
    painter.drawText(textX, textY + textRect.height() - painter.fontMetrics().descent(), QString::number(fret));
}

// Mouse move event
void TabFret::mouseMoveEvent(QMouseEvent *event)
{
    if (state == State::Hovered) return;
    if (state != State::Selected) state = State::Hovered;
    update();
    emit mouseMoved();
}

// Leave event
void TabFret::leaveEvent(QEvent *event)
{
    if (state != State::Selected) state = State::Normal;
    update();
    emit mouseLeft();
}

// Toggle slot
void TabFret::onToggle(bool checked)
{
    state = (checked) ? State::Selected : State::Normal;
    update();
}

// Column state change slot
void TabFret::onColumnStateChange(State state)
{
    textBackground = columnStateColours[state];
    update();
}
