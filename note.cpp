#include "note.h"

#include <QPainter>
#include <QPainterPath>

#define INVALID_LINE -999
#define STEM_HEIGHT 56
#define HEAD_WIDTH 17
#define HEAD_HEIGHT 11
#define STAFF_SPACING 7.5

Note::Note(QVector<int> staffLines, QWidget *parent)
    : staffLines{staffLines}, QWidget{parent}
{
    setFixedWidth(35);
}

// Draws the entire note
void Note::paintEvent(QPaintEvent *event)
{
    QVector<int> yPos;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);

    for (int staffLine : staffLines)
    {
        if (staffLine == INVALID_LINE) continue;
        painter.save();

        flip = staffLine > 0;
        addLines = abs(staffLine) > 5;

        // Translate the painter to the center of the widget with the given Y offset
        int offsetY = STAFF_SPACING * -staffLine;
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
        if (isSingleNote()) drawSingleStem();
        if (addLines) drawExtraLines(staffLine);

        painter.restore();
    }
    // Draw the note stem of chords
    if (!isSingleNote()) drawMultiStem(yPos);

    // TEST
    drawFlag(yPos);

    painter.end();
}

// Getter for staff lines
QVector<int> Note::getStaffLines()
{
    return staffLines;
}

// Draws extra lines outside the staff
void Note::drawExtraLines(int staffLine)
{
    // Don't draw if extra lines already exist
    int min = *std::min_element(staffLines.begin(), staffLines.end(), [](int a, int b) { return (a != INVALID_LINE && (b == INVALID_LINE || a < b)); });
    int max = *std::max_element(staffLines.begin(), staffLines.end());
    if ((staffLine < 0 && staffLine != min) || (staffLine > 0 && staffLine != max)) {
        return;
    }

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
        int yPoint = STAFF_SPACING * i + offset;
        painter.drawLine((HEAD_WIDTH / 2) + 3, -yPoint, (-HEAD_WIDTH / 2) - 3, -yPoint);
    }
}

// Draws the stem of singular notes
void Note::drawSingleStem()
{
    int staffLine = *std::find_if(std::begin(staffLines), std::end(staffLines), [](int x) { return x != INVALID_LINE; });
    int stemHeight = STEM_HEIGHT + std::max(0, abs(staffLine) - 7) * STAFF_SPACING;

    painter.setPen(QPen(Qt::white, 1.5));
    painter.drawLine((HEAD_WIDTH / 2), (HEAD_HEIGHT / 2) - stemHeight, (HEAD_WIDTH / 2), (HEAD_HEIGHT / 2) - 8);
}

// Draws the stem of multiple notes
void Note::drawMultiStem(QVector<int> yPos)
{
    int maxPos = *std::max_element(yPos.begin(), yPos.end());
    int minPos = *std::min_element(yPos.begin(), yPos.end());
    int maxLine = *std::max_element(staffLines.begin(), staffLines.end());
    int minLine = *std::min_element(staffLines.begin(), staffLines.end(), [](int a, int b) {
        return (a != INVALID_LINE) && (b == INVALID_LINE || a < b); });

    int stemHeight = (maxLine < -7) ? STEM_HEIGHT + (abs(minLine) - 7) * STAFF_SPACING : maxPos - minPos + STEM_HEIGHT;

    painter.setPen(QPen(Qt::white, 1.5));
    painter.translate(width() / 2, maxPos);
    painter.drawLine((HEAD_WIDTH / 2), (HEAD_HEIGHT / 2) - stemHeight, (HEAD_WIDTH / 2), (HEAD_HEIGHT / 2) - 10);
}

// Draws the flag of the note
void Note::drawFlag(QVector<int> yPos)
{
    QPainterPath path;
    qDebug() << yPos;

    // Define the first arc
    QRectF rect1(50, 100, 100, 100);
    path.arcTo(rect1, 0, 180); // Start at 0 degrees, span 180 degrees

    // Define the second arc
    QRectF rect2(150, 100, 100, 100);
    path.arcTo(rect2, 180, 180); // Start at 180 degrees, span 180 degrees

    // Close the path to connect the arcs
    path.lineTo(150, 200); // Connect to the bottom of the second arc
    path.lineTo(50, 200);  // Connect to the bottom of the first arc
    path.closeSubpath();    // Close the shape

    // Translate path to start at the top of stem
    path.translate(width() / 2, yPos[0]);   // TODO: Change to account for multiple flags
    path.translate((HEAD_WIDTH / 2), (HEAD_HEIGHT / 2) - STEM_HEIGHT);

    // Set pen and fill color
    painter.setPen(Qt::black);
    painter.setBrush(Qt::cyan); // Fill color

    // Draw the path
    painter.drawPath(path);
}

// Checks whether the note is singular
bool Note::isSingleNote()
{
    int count = 0;
    for (int line : staffLines)
    {
        if (line != INVALID_LINE) count++;
    }
    return (count == 1);
}
