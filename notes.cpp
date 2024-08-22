#include "notes.h"

#include <QPainter>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>

// ========== Blank Class ==========
Blank::Blank(QWidget *parent)
    : QWidget{parent}
{
    setFixedWidth(35);
}

// ========== Crotchet Class ==========
Crotchet::Crotchet(int staffLine, QWidget *parent)
    : staffLine{staffLine}, QWidget{parent}
{
    setFixedWidth(35);
    flip = (staffLine > 0) ? true : false;
    addLines = (abs(staffLine) > 5) ? true : false;
}

// Draws the note head
void Crotchet::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);

    // Translate the painter to the center of the widget with the given Y offset
    int offsetY = 7.5 * -staffLine;
    painter.translate(width() / 2, height() / 2 + offsetY);

    // Flip crotchet if above middle line
    if (flip) painter.scale(-1, -1);

    painter.save();
    painter.rotate(-30);

    // Draw the note head centered at (0, 0)
    int headWidth = 21;
    int headHeight = 13;
    QRect rect(-headWidth / 2, (-headHeight / 2), headWidth, headHeight);
    painter.drawEllipse(rect);

    // Draw the note stem
    int stemHeight = 58;
    painter.restore();
    painter.setPen(QPen(Qt::white, 1.5));
    painter.drawLine((headWidth / 2), (headHeight / 2) - stemHeight, (headWidth / 2), (headHeight / 2) - 10);

    // Draw additional lines if necessary
    if (addLines)
    {
        painter.setPen(QPen(Qt::white, 2));
        int numLines = abs(staffLine) - 4;
        int offset = 0;

        // Note is not on a line
        if (numLines % 2 != 0)
        {
            offset = headHeight / 2 + 1;
            numLines--;
        }
        // Draw lines
        for (int i = 0; i < numLines; i += 2)
        {
            int yPoint = 7.5 * i + offset;
            painter.drawLine((headWidth / 2) + 3, -yPoint, (-headWidth / 2) - 3, -yPoint);
        }
    }
}



