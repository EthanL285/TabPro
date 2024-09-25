#include "notes.h"

#include <QPainter>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>

#define INVALID_LINE -999
#define STEM_HEIGHT 50
#define HEAD_WIDTH 21
#define HEAD_HEIGHT 13

// ========== Blank Class ==========
Blank::Blank(QWidget *parent)
    : QWidget{parent}
{
    setFixedWidth(35);
}

// ========== Crotchet Class ==========
Crotchet::Crotchet(QVector<int> staffLines, QWidget *parent)
    : staffLines{staffLines}, QWidget{parent}
{
    setFixedWidth(35);
}

// Returns the staff lines of all the crotchet heads
QVector<int> Crotchet::getStaffLines()
{
    return staffLines;
}

// Draws the note head
void Crotchet::paintEvent(QPaintEvent *event)
{
    QVector<int> yPos;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);

    for (int staffLine : staffLines)
    {
        if (staffLine == INVALID_LINE) continue;
        painter.save();

        flip = staffLine > 0;
        addLines = abs(staffLine) > 5;

        // Translate the painter to the center of the widget with the given Y offset
        int offsetY = 7.5 * -staffLine;
        yPos.append(height() / 2 + offsetY);
        painter.translate(width() / 2, height() / 2 + offsetY);

        // Flip crotchet if above middle line
        if (isSingleNote() && flip) painter.scale(-1, -1);

        painter.save();
        painter.rotate(-30);

        // Draw the note head centered at (0, 0)
        QRect rect(-HEAD_WIDTH / 2, (-HEAD_HEIGHT / 2), HEAD_WIDTH, HEAD_HEIGHT);
        painter.drawEllipse(rect);
        painter.restore();

        // Draw the note stem and additional lines
        if (isSingleNote()) drawSingleStem(painter);
        if (addLines) drawAdditionalLines(painter, staffLine);

        painter.restore();
    }

    // Draw the note stem of chords
    if (!isSingleNote()) drawChordStem(painter, yPos);
}

// Checks whether the crotchet is a single note
bool Crotchet::isSingleNote()
{
    int count = 0;
    for (int line : staffLines)
    {
        if (line != INVALID_LINE) count++;
    }
    return (count == 1);
}

// Draws additional lines outside the staff
void Crotchet::drawAdditionalLines(QPainter &painter, int staffLine)
{
    int numLines = abs(staffLine) - 4;
    int offset = 0;

    if (!isSingleNote() && flip) painter.scale(-1, -1);
    painter.setPen(QPen(Qt::white, 2));

    // Note is not on a line
    if (numLines % 2 != 0)
    {
        offset = HEAD_HEIGHT / 2 + 1;
        numLines--;
    }
    // Draw lines
    for (int i = 0; i < numLines; i += 2)
    {
        int yPoint = 7.5 * i + offset;
        painter.drawLine((HEAD_WIDTH / 2) + 3, -yPoint, (-HEAD_WIDTH / 2) - 3, -yPoint);
    }
}

// Draws the note stem of single crotchets
void Crotchet::drawSingleStem(QPainter &painter)
{
    painter.setPen(QPen(Qt::white, 1.5));
    painter.drawLine((HEAD_WIDTH / 2), (HEAD_HEIGHT / 2) - STEM_HEIGHT, (HEAD_WIDTH / 2), (HEAD_HEIGHT / 2) - 10);
}

// Draws the note stem of chord crotchets
void Crotchet::drawChordStem(QPainter &painter, QVector<int> yPos)
{
    int max = *std::max_element(yPos.begin(), yPos.end());
    int min = *std::min_element(yPos.begin(), yPos.end());
    int offset = max - min + STEM_HEIGHT;

    painter.setPen(QPen(Qt::white, 1.5));
    painter.translate(width() / 2, max);
    painter.drawLine((HEAD_WIDTH / 2), (HEAD_HEIGHT / 2) - offset, (HEAD_WIDTH / 2), (HEAD_HEIGHT / 2) - 10);
}



