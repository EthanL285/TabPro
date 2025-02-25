#include "barline.h"
#include "tablature.h"
#include <QPainter>

BarLine::BarLine(int height)
{
    setFixedWidth(Tablature::DEFAULT_COLUMN_WIDTH);
    barHeight = height;
}

static const int NUM_LINES = 6;

// Paint event
void BarLine::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Draw bar line
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::white, 2.5));
    painter.translate(width() / 2, height() / 2);
    painter.drawLine(0, -barHeight, 0, barHeight);

    if (barHeight != Tablature::BARLINE_HEIGHT) return;

    // Draw tab lines
    painter.setPen(QPen(Qt::white, 1.5));
    painter.setTransform(QTransform());

    int topPadding = 22;
    int availableHeight = height() - 2 * topPadding;
    int spacing = availableHeight / (NUM_LINES - 1);

    for (int i = 0; i < NUM_LINES; i++)
    {
        int y = topPadding + i * spacing;
        painter.drawLine(0, y, width(), y);
    }
}

