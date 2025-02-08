#include "tablatureindent.h"
#include "tablature.h"

#include <QPainter>

TablatureIndent::TablatureIndent(int width, QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(width, 205);
}

// Paint event
void TablatureIndent::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);;
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::white, 1.5));

    // Draw tab lines
    int topPadding = 22;
    int availableHeight = height() - 2 * topPadding;
    int spacing = availableHeight / (Tablature::NUM_LINES - 1);

    for (int i = 0; i < Tablature::NUM_LINES; i++)
    {
        int y = topPadding + i * spacing;
        painter.drawLine(0, y, width(), y);
    }
}
