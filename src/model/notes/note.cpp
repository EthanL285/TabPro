#include "note.h"
#include "staff.h"

#include <QPainter>
#include <QPainterPath>

Note::Note(QVector<int> staffLines)
    : staffLines{staffLines}
{
    setFixedWidth(35);
}

// Getter for staff lines
QVector<int> Note::getStaffLines()
{
    return staffLines;
}

// Getter for type
NoteType Note::getType()
{
    return type;
}

// Toggles selection of note
void Note::toggleSelect()
{
    isSelected = !isSelected;
    update();
}

// Draws the note head
void Note::drawNoteHead(QPainter &painter)
{
    if (isSelected)
    {
        QPen pen(QColor(70, 129, 232));
        pen.setWidth(2);
        painter.setPen(pen);
        painter.setBrush(QColor(70, 129, 232));
    }
    QRect rect(-HEAD_WIDTH / 2, (-HEAD_HEIGHT / 2), HEAD_WIDTH, HEAD_HEIGHT);
    painter.rotate(-30);
    painter.drawEllipse(rect);
}

// Draws extra lines outside the staff
void Note::drawExtraLines(int staffLine)
{
    // Don't draw if extra lines already exist
    int min = *std::min_element(staffLines.begin(), staffLines.end(), [](int a, int b) { return (a != Staff::Staff::INVALID_LINE && (b == Staff::INVALID_LINE || a < b)); });
    int max = *std::max_element(staffLines.begin(), staffLines.end());
    if ((staffLine < 0 && staffLine != min) || (staffLine > 0 && staffLine != max)) {
        return;
    }

    int numLines = abs(staffLine) - 4;
    int offset = 0;

    if (!isSingleNote() && staffLine > 0) painter.scale(-1, -1);
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
        int yPoint = Staff::Staff::STAFF_SPACING * i + offset;
        painter.drawLine((HEAD_WIDTH / 2) + 3, -yPoint, (-HEAD_WIDTH / 2) - 3, -yPoint);
    }
}

// Draws the stem of singular notes
void Note::drawSingleStem()
{
    int staffLine = *std::find_if(std::begin(staffLines), std::end(staffLines), [](int x) { return x != Staff::INVALID_LINE; });
    stemHeight = STEM_HEIGHT + std::max(0, abs(staffLine) - 7) * Staff::STAFF_SPACING;

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
        return (a != Staff::INVALID_LINE) && (b == Staff::INVALID_LINE || a < b); });

    stemHeight = (maxLine < -7) ? STEM_HEIGHT + (abs(minLine) - 7) * Staff::STAFF_SPACING : maxPos - minPos + STEM_HEIGHT;

    painter.save();
    painter.setPen(QPen(Qt::white, 1.5));
    painter.translate(width() / 2, maxPos);
    painter.drawLine((HEAD_WIDTH / 2), (HEAD_HEIGHT / 2) - stemHeight, (HEAD_WIDTH / 2), (HEAD_HEIGHT / 2) - 10);
    painter.restore();
}

// Checks whether the note is singular
bool Note::isSingleNote()
{
    int count = 0;
    for (int line : staffLines)
    {
        if (line != Staff::INVALID_LINE) count++;
    }
    return count == 1;
}

// Checks whether the note is flipped
bool Note::isFlipped()
{
    int staffLine = *std::find_if(staffLines.begin(), staffLines .end(), [](int x) { return x != Staff::INVALID_LINE; } );
    return isSingleNote() && staffLine > 0;
}
