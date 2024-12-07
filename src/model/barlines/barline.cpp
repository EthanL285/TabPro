#include "barline.h"
#include <QPainter>

#define HEIGHT 28

BarLine::BarLine()
{
    setFixedWidth(35);
}

// Paint event
void BarLine::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen(Qt::white, 2.5);
    painter.setPen(pen);

    painter.translate(width() / 2, height() / 2);
    painter.drawLine(0, -HEIGHT, 0, HEIGHT);
}

