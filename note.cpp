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
    // setFixedWidth(35);
    setFixedWidth(47);
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
    stemHeight = STEM_HEIGHT + std::max(0, abs(staffLine) - 7) * STAFF_SPACING;

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

    stemHeight = (maxLine < -7) ? STEM_HEIGHT + (abs(minLine) - 7) * STAFF_SPACING : maxPos - minPos + STEM_HEIGHT;

    painter.setPen(QPen(Qt::white, 1.5));
    painter.translate(width() / 2, maxPos);
    painter.drawLine((HEAD_WIDTH / 2), (HEAD_HEIGHT / 2) - stemHeight, (HEAD_WIDTH / 2), (HEAD_HEIGHT / 2) - 10);
}

// Draws the flag of the note
void Note::drawFlag(QVector<int> yPos)
{
    QPainterPath path;

    // Draw bezier curves
    //     - Origin Point: (109, 50)
    //     - https://ytyt.github.io/siiiimple-bezier/?e=linear&s=1000&w=500&h=500&c=109,50,116,115,215,109,165,260 (outer curve)
    //     - https://ytyt.github.io/siiiimple-bezier/?e=linear&s=1000&w=500&h=500&c=109,111,143,119,191,151,159,260 (inner curve)

    // Outer curve
    QPointF c1(7, 65);
    QPointF c2(106, 59);
    QPoint end1(56, 210);

    // Inner curve
    QPointF c3(82, 101);
    QPointF c4(39, 69);
    QPointF end2(0, 61);

    // Scale control and end points
    double scaleFactor = 0.22;
    double yFactor = (flip) ? -scaleFactor : scaleFactor;

    c1.setX(c1.x() * scaleFactor);
    c1.setY(c1.y() * yFactor);
    c2.setX(c2.x() * scaleFactor);
    c2.setY(c2.y() * yFactor);
    end1.setX(end1.x() * scaleFactor);
    end1.setY(end1.y() * yFactor);

    c3.setX(c3.x() * scaleFactor);
    c3.setY(c3.y() * yFactor);
    c4.setX(c4.x() * scaleFactor);
    c4.setY(c4.y() * yFactor);
    end2.setX(end2.x() * scaleFactor);
    end2.setY(end2.y() * yFactor);

    path.cubicTo(c1, c2, end1);
    path.cubicTo(c3, c4, end2);

    // Translate path to start at the top of stem    
    float xOffset = flip ? -HEAD_WIDTH / 2 : HEAD_WIDTH / 2;
    float yOffset = flip ? -HEAD_HEIGHT / 2 + stemHeight : HEAD_HEIGHT / 2 - stemHeight;
    path.translate(xOffset, yOffset);
    path.translate(width() / 2, yPos[0]);

    // Draw the path
    painter.setPen(Qt::white);
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
