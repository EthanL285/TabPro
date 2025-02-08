#include "tablaturebutton.h"
#include "tablature.h"

#include <QPainter>

TablatureButton::TablatureButton(QWidget *parent)
    : QPushButton{parent}
{
    updateText(EMPTY_COLUMN);
    setFixedSize(35, 205);
    setCheckable(true);
    setCursor(Qt::PointingHandCursor);
    setFocusPolicy(Qt::NoFocus);
    setStyleSheet
    (
        "QPushButton { "
        "   border-radius: 1px;"
        "   background-color: rgb(23,23,23);"
        "   font: 20pt Consolas;"
        "}"
        "QPushButton:hover { "
        "   background-color: rgb(75,75,75);"
        "}"
    );
}

// Paint event
void TablatureButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);;
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw button state
    painter.setPen(Qt::NoPen);
    painter.setBrush(isHovered ? QColor(75,75,75) : (isSelected ? QColor(50,50,50) : Qt::transparent));
    painter.drawRect(0, 0, width(), height());

    // Draw tab lines
    int topPadding = 22;
    int availableHeight = height() - 2 * topPadding;
    int spacing = availableHeight / (Tablature::NUM_LINES - 1);
    painter.setPen(QPen(Qt::white, 1.5));

    for (int i = 0; i < Tablature::NUM_LINES; i++)
    {
        int y = topPadding + i * spacing;
        painter.drawLine(0, y, width(), y);
    }
    // Draw fret numbers
    painter.setFont(QFont("Consolas", 20));
    painter.setBrush(Qt::NoBrush);

    for (int i = 0; i < Tablature::NUM_LINES; i++)
    {
        if (fretNumbers[i] == "\u2015") continue;

        // Calculate size and position of text
        QRect textRect = painter.boundingRect(rect(), Qt::AlignCenter, fretNumbers[i]);
        int textX = (width() - textRect.width()) / 2;
        int textY = (topPadding + i * spacing) - (textRect.height() / 2) - 1;

        // Draw text and background
        painter.save();
        painter.setBrush(isHovered ? QColor(75,75,75) : (isSelected ? QColor(50,50,50) : QColor(23,23,23)));
        painter.setPen(QPen(Qt::NoPen));
        painter.drawRect(textX, textY, textRect.width(), textRect.height());
        painter.restore();
        painter.drawText(textX, textY + textRect.height() - painter.fontMetrics().descent(), fretNumbers[i]);
    }
}

// Toggles the button selected state
void TablatureButton::toggleSelect()
{
    isSelected = !isSelected;
    update();
}

// Updates the button text
void TablatureButton::updateText(QString text)
{
    setText(text);
    fretNumbers = text.split("\n", Qt::SkipEmptyParts);
}

// Mouse move event
void TablatureButton::mouseMoveEvent(QMouseEvent *event)
{
    if (isHovered) return;
    isHovered = true;
    update();
}

// Leave event
void TablatureButton::leaveEvent(QEvent *event)
{
    isHovered = false;
    update();
}

