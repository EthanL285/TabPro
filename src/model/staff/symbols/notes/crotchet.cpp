#include "crotchet.h"
#include "staff.h"

Crotchet::Crotchet(QVector<int> staffLines)
    : Note(staffLines)
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
        if (staffLine == Staff::INVALID_LINE) continue;
        painter.save();

        // Translate the painter to the center of the widget with the given y offset
        int offsetY = Staff::STAFF_SPACING * -staffLine;
        yPos.append(height() / 2 + offsetY);
        painter.translate(width() / 2, height() / 2 + offsetY);

        // Flip note
        if (isFlipped()) painter.scale(-1, -1);

        // Draw the note stem and additional lines
        if (isSingleNote()) drawSingleStem();
        if (abs(staffLine) > 5) drawExtraLines(staffLine);

        // Draw the note head centered at (0, 0)
        drawNoteHead(painter);
        painter.restore();
    }
    // Draw the note stem of chords
    if (!isSingleNote()) drawMultiStem(yPos);

    updateHeight();
    painter.end();
}

// Get the beat value of the note
double Crotchet::getBeatValue()
{
    return 1;
}
