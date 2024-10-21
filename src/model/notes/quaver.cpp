#include "quaver.h"

#include <QPainterPath>

Quaver::Quaver(QVector<int> staffLines, QWidget *parent)
    : Note(staffLines, parent)
{
    type = NoteType::Quaver;
}

// Draws the quaver
void Quaver::paintEvent(QPaintEvent *event)
{
    yPos.clear();
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);

    // Translate left to include flag
    if (!isFlipped()) painter.translate(-6,0);

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

    drawFlag();
    painter.end();
}

// Draws the flag of the quaver
void Quaver::drawFlag()
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
    double yFactor = (isFlipped()) ? -scaleFactor : scaleFactor;

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
    float xOffset = (isFlipped()) ? -HEAD_WIDTH / 2 : HEAD_WIDTH / 2;
    float yOffset = (isFlipped()) ? -HEAD_HEIGHT / 2 + stemHeight : HEAD_HEIGHT / 2 - stemHeight;
    path.translate(xOffset, yOffset);
    path.translate(width() / 2, *std::max_element(yPos.begin(), yPos.end()));

    // Draw the path
    painter.setPen(Qt::white);
    painter.drawPath(path);
}
