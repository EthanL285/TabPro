#include "barline.h"
#include <QPainter>

BarLine::BarLine(int height)
{
    setFixedWidth(35);
    barHeight = height;
}

// Paint event
void BarLine::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen(Qt::white, 2.5);
    painter.setPen(pen);

    painter.translate(width() / 2, height() / 2);
    painter.drawLine(0, -barHeight, 0, barHeight);
}

