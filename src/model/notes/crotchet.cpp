#include "crotchet.h"

Crotchet::Crotchet(QVector<int> staffLines, QWidget *parent)
    : Note(staffLines, parent)
{
    type = NoteType::Crotchet;
}

// Draws the quaver
void Crotchet::paintEvent(QPaintEvent *event)
{
    yPos.clear();
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);

    for (int staffLine : staffLines)
    {
        if (staffLine == INVALID_LINE) continue;
        painter.save();

        // Translate the painter to the center of the widget with the given y offset
        int offsetY = STAFF_SPACING * -staffLine;
        yPos.append(height() / 2 + offsetY);
        painter.translate(width() / 2, height() / 2 + offsetY);

        // Flip note
        if (isFlipped()) painter.scale(-1, -1);

        painter.save();
        painter.rotate(-30);

        // Draw the note head centered at (0, 0)
        QRect rect(-HEAD_WIDTH / 2, (-HEAD_HEIGHT / 2), HEAD_WIDTH, HEAD_HEIGHT);
        painter.drawEllipse(rect);
        painter.restore();

        // Draw the note stem and additional lines
        if (isSingleNote()) drawSingleStem();
        if (abs(staffLine) > 5) drawExtraLines(staffLine);

        painter.restore();
    }
    // Draw the note stem of chords
    if (!isSingleNote()) drawMultiStem(yPos);

    painter.end();
}
