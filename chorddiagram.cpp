#include "chorddiagram.h"

ChordDiagram::ChordDiagram(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *diagramLayout = new QVBoxLayout(this);
    setFixedWidth(250);
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    // Open and Close String Buttons
    QHBoxLayout *openCloseLayout = new QHBoxLayout();
    openCloseLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    openCloseLayout->setSpacing(30);
    diagramLayout->addLayout(openCloseLayout);

    for (int i = 0; i < 6; i++)
    {
        QPushButton *openClose = new QPushButton();
        openClose->setCheckable(true);
        openClose->setFixedSize(14,14);
        openClose->setCursor(Qt::PointingHandCursor);
        openClose->setStyleSheet
        (
            "QPushButton {"
            "    border: 1px solid white;"
            "    border-radius: 7px;"
            "    background-color: transparent;"
            "}"
            "QPushButton:hover {"
            "    border: 1.5px solid white;"
            "    font-weight: bold;"
            "}"
            "QPushButton:checked {"
            "    border: none;"
            "    font-family: Muli;"
            "    font-size: 13pt;"
            "}"
        );
        openCloseLayout->addWidget(openClose);
        connect(openClose, &QPushButton::clicked, this, [openClose]()
        {
            if (openClose->isChecked()) openClose->setText("X");
            else openClose->setText("");
        });
        stringButtons.append(openClose);
    }

    // String labels
    QHBoxLayout *stringLayout = new QHBoxLayout();
    stringLayout->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    stringLayout->setSpacing(32);
    diagramLayout->addLayout(stringLayout);

    QVector<QString> strings = {"E","A","D","G","B","E"};
    for (int i = 0; i < 6; i++)
    {
        QLabel *letter = new QLabel(strings[i]);
        letter->setStyleSheet("color: white; font: 11pt Muli; border: none;");
        stringLayout->addWidget(letter);
    }
}

// Paint event
void ChordDiagram::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::white, 0.5));
    painter.save();

    // Set values
    paddingLeftRight = 15;
    paddingTop = 35;
    paddingBottom = 35;
    cellWidth = (width() - 2 * paddingLeftRight) / COLS;
    cellHeight = (height() - paddingTop - paddingBottom) / ROWS;

    // Initialise snap positions
    if (snapPositions.empty()) setSnapPositions();

    // Draw horizontal lines
    painter.setPen(QPen(Qt::white, 5, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(paddingLeftRight, paddingTop, width() - paddingLeftRight, paddingTop);
    painter.restore();

    for (int i = 1; i < ROWS; i++)
    {
        int y = i * cellHeight + paddingTop;
        painter.drawLine(paddingLeftRight, y, width() - paddingLeftRight, y);
    }
    painter.drawLine (
        paddingLeftRight, height() - paddingBottom,
        width() - paddingLeftRight, height() - paddingBottom
    );

    // Draw vertical lines
    for (int i = 0; i <= COLS; i++)
    {
        int x = i * cellWidth + paddingLeftRight;
        painter.drawLine(x, paddingTop, x, height() - paddingBottom);
    }

    QFont font("Muli", 10);
    font.setBold(true);
    painter.setFont(font);
    painter.setBrush(QBrush(QColor(80,80,80), Qt::SolidPattern));
    painter.setPen(QPen(QColor(45,45,45), 1));

    // Draw bar
    if (barExists) drawBar(font, painter);

    // Draw hover and currently placed circles
    drawPlacedCircles(painter);
    drawHoverCircle(painter);
}

// Draws the bar
void ChordDiagram::drawBar(QFont font, QPainter &painter)
{
    int factor = NUM_STRINGS - barSpan;
    int rectWidth = width() - factor * cellWidth;
    int rectHeight = 28;
    int barOffSet = factor * (cellWidth / 2);
    int rectX = (width() - rectWidth) / 2 + barOffSet;
    int rectY = paddingTop + (cellHeight / 2) - (rectHeight / 2);
    painter.drawRoundedRect(rectX, rectY, rectWidth, rectHeight, 14, 14);

    QFontMetrics fontMetrics(font);
    QString text = QString::number(barPlacement);
    QRect textRect = fontMetrics.boundingRect(text);
    int textOffSet = factor * cellWidth;
    int textX = paddingLeftRight - (fontMetrics.horizontalAdvance(text) / 2) + textOffSet;
    int textY = rectY + (rectHeight - textRect.height()) / 2 + fontMetrics.ascent();
    painter.save();
    painter.setPen(Qt::white);
    painter.drawText(textX, textY, text);
    painter.restore();
}

// Draws all the currently placed circles
void ChordDiagram::drawPlacedCircles(QPainter &painter)
{
    for (int i = 1; i <= MAX_CIRCLES; i++)
    {
        if (!placedCircles.contains(i) || placedCircles[i] == grabbedCircle) continue;

        drawCircle(painter, placedCircles[i], i);
        setStringVisibility(getStringNumber(placedCircles[i]), false);
    }
}

// Draws the hover circle
void ChordDiagram::drawHoverCircle(QPainter &painter)
{
    if (!isWidgetHovered || currCirclePos.isNull()) return;

    // Determine the next circle number based off mode
    int circleNum = 0;
    if (placeMode && !limitReached) circleNum = getNextCircleNumber();
    else if (dragMode && isPressed) circleNum = getCircleNumber(grabbedCircle);

    // Draw circle if circle number is valid
    if (circleNum != 0) drawCircle(painter, currCirclePos, circleNum);
}

// Draws a circle with the given number
void ChordDiagram::drawCircle(QPainter &painter, QPointF center, int circleNum)
{
    painter.save();
    QRectF textRect(QPoint(10, 10), QSize(20, 20));
    textRect.moveCenter(center);
    painter.drawEllipse(center, 14, 14);
    painter.setPen(Qt::white);
    painter.drawText(textRect, Qt::AlignCenter, QString::number(circleNum));
    painter.restore();
}

// Initialises a vector of all snap positions
void ChordDiagram::setSnapPositions()
{
    for (int i = 0; i <= COLS; i++)
    {
        int xPos = i * cellWidth + paddingLeftRight;
        for (int i = 0; i < ROWS; i++)
        {
            int yPos = i * cellHeight + paddingTop + (cellHeight / 2);
            snapPositions.append(QPointF(xPos, yPos));
        }
    }
    barDeletePoint = QPointF(paddingLeftRight, paddingTop + (cellHeight / 2));
    snapPositions.append(barDeletePoint);
}

// Hover event
void ChordDiagram::mouseMoveEvent(QMouseEvent *event)
{
    // Get position of mouse
    currCirclePos = snapToGrid(event->pos());
    if (!snap) currCirclePos = event->pos();
    isWidgetHovered = !(currCirclePos.y() < 35 || currCirclePos.y() > height() - 35);
    isCircleHovered = isHoveringCircle(currCirclePos);

    // Set cursor
    if (placeMode) setCursor(isWidgetHovered && !limitReached ? Qt::PointingHandCursor : Qt::ArrowCursor);
    else if (dragMode && !isPressed) setCursor(isCircleHovered && snap ? Qt::OpenHandCursor : Qt::ArrowCursor);
    else if (deleteMode) setCursor(isCircleHovered && snap ? Qt::PointingHandCursor : Qt::ArrowCursor);
    else if (!placeMode && !dragMode && !deleteMode) setCursor(Qt::ArrowCursor);

    update();
}

// Snaps the circle to a position on the grid
QPointF ChordDiagram::snapToGrid(const QPointF &pos)
{
    // Calculate the shortest euclidean distance
    QPointF nearestPoint;
    double minDistance = std::numeric_limits<double>::max();

    for (const auto &point : snapPositions)
    {
        double distance = std::sqrt(std::pow(point.x() - pos.x(), 2) + std::pow(point.y() - pos.y(), 2));
        if (distance < minDistance)
        {
            minDistance = distance;
            nearestPoint = point;
        }
    }
    bool isCloseEnough = (minDistance <= 13);
    bool isNotBarFret = (barPlacement == 0 || getFretNumber(nearestPoint) != 1);
    bool isInDeleteMode = (deleteMode && nearestPoint == barDeletePoint);
    snap = isCloseEnough && (isNotBarFret || isInDeleteMode);

    return nearestPoint;
}

// Leave event
void ChordDiagram::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    isWidgetHovered = false;
    update();
}

// Press Event
void ChordDiagram::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || !snap) return;

    // Handle mode press event
    if (placeMode && !limitReached) handlePlaceMode();
    else if (dragMode && isCircleHovered) handleDragMode();
    else if (deleteMode && isCircleHovered) handleDeleteMode();

    update();
}

// Handles the place mode press event
void ChordDiagram::handlePlaceMode()
{
    int num = getNextCircleNumber();
    int sameStringCircle = getSameStringCircle(currCirclePos);

    if (sameStringCircle != -1) placedCircles.remove(sameStringCircle);

    placedCircles.insert(num, currCirclePos);
    limitReached = (placedCircles.size() >= MAX_CIRCLES);
}

// Handles the drag mode press event
void ChordDiagram::handleDragMode()
{
    grabbedCircle = currCirclePos;
    isPressed = true;
    setCursor(Qt::ClosedHandCursor);
}

// Handles the delete mode press event
void ChordDiagram::handleDeleteMode()
{
    int circleNum = getCircleNumber(currCirclePos);
    int barIdx = snapPositions.indexOf(barDeletePoint);

    if (circleNum != -1)
    {
        setStringVisibility(getStringNumber(currCirclePos), true);
        placedCircles.remove(circleNum);
        limitReached = false;
        setCursor(Qt::ArrowCursor);
    }
    else if (barExists && currCirclePos == barDeletePoint && barSpan > 2)
    {
        barSpan--;
        snapPositions.remove(barIdx);
        barDeletePoint.setX(paddingLeftRight + (NUM_STRINGS - barSpan) * cellWidth);
        snapPositions.append(barDeletePoint);
        setCursor(Qt::ArrowCursor);
    }
}

// Release Event
void ChordDiagram::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || !dragMode) return;

    QPointF newPoint = snapToGrid(event->pos());
    int circleNum = getCircleNumber(grabbedCircle);
    int duplicateCircle = getCircleNumber(newPoint);
    int sameStringCircle = getSameStringCircle(newPoint);

    if (snap && isPressed)
    {
        handleDragPlacement(circleNum, sameStringCircle, duplicateCircle, newPoint);
    }
    isPressed = false;
    grabbedCircle = INVALID_POINT;
    setCursor(isCircleHovered ? Qt::OpenHandCursor : Qt::ArrowCursor);

    update();
}

// Handles the drag mode release event
void ChordDiagram::handleDragPlacement(int circleNum, int sameStringCircle, int duplicateCircle, QPointF newPoint)
{
    if (sameStringCircle != -1)
    {
        handleSameStringCircle(circleNum, sameStringCircle, newPoint);
    }
    else if (duplicateCircle != -1 && duplicateCircle != circleNum)
    {
        handleDuplicateCircle(circleNum, sameStringCircle, duplicateCircle);
    }
    else
    {
        handleValidPlacement(circleNum, newPoint);
    }
}

// Handles the drag case where circles are placed on the same string
void ChordDiagram::handleSameStringCircle(int circleNum, int sameStringCircle, QPointF newPoint)
{
    placedCircles.insert(sameStringCircle, placedCircles[circleNum]);
    placedCircles.insert(circleNum, newPoint);
}

// Handles the drag case where a circle is placed on an existing circle
void ChordDiagram::handleDuplicateCircle(int circleNum, int sameStringCircle, int duplicateCircle)
{
    QPointF temp = placedCircles[sameStringCircle];
    placedCircles.insert(sameStringCircle, placedCircles[circleNum]);
    placedCircles.insert(circleNum, temp);
}

// Handles valid drag placement
void ChordDiagram::handleValidPlacement(int circleNum, QPointF newPoint)
{
    int newStringNum = getStringNumber(newPoint);
    int oldStringNum = getStringNumber(placedCircles[circleNum]);

    setStringVisibility(newStringNum, false);
    setStringVisibility(oldStringNum, true);
    placedCircles.insert(circleNum, newPoint);
}

// Places the bar on the diagram
void ChordDiagram::placeBar()
{
    QComboBox* barDropdown = qobject_cast<QComboBox*>(sender());
    barPlacement = barDropdown->currentIndex();
    barSpan = NUM_STRINGS;

    // Reset bar delete point
    snapPositions.removeAll(barDeletePoint);
    barDeletePoint = QPointF(paddingLeftRight, paddingTop + (cellHeight / 2));
    snapPositions.append(barDeletePoint);

    // Remove existing bar
    if (barPlacement == 0)
    {
        placedCircles.remove(1);
    }
    else
    {
        // Place the bar and remove any fret 1 circles
        placedCircles.insert(1, barPoint);
        for (int i = 2; i <= MAX_CIRCLES; i++)
        {
            if (placedCircles.contains(i) && getFretNumber(placedCircles[i]) == 1)
            {
                placedCircles.remove(i);
            }
        }
    }
    barExists = (barPlacement != 0);
    limitReached = (placedCircles.size() >= MAX_CIRCLES);

    update();
}

// Checks whether a point is on the same string as a placed circle
int ChordDiagram::getSameStringCircle(QPointF point)
{
    for (int i = 1; i <= MAX_CIRCLES; i++)
    {
        if (placedCircles.contains(i) && placedCircles[i].x() == point.x()) return i;
    }
    return -1;
}

// Returns the circle number at the given coordinates
int ChordDiagram::getCircleNumber(QPointF point)
{
    for (int i = 1; i <= MAX_CIRCLES; i++)
    {
        if (placedCircles.contains(i) && placedCircles[i] == point) return i;
    }
    return -1;
}

// Returns the next circle number to be placed
int ChordDiagram::getNextCircleNumber()
{
    for (int i = 1; i <= MAX_CIRCLES; i++)
    {
        if (!placedCircles.contains(i)) return i;
    }
    return MAX_CIRCLES;
}

// Checks whether cursor is hovering a circle
bool ChordDiagram::isHoveringCircle(QPointF point)
{
    if (barPlacement != 0 && point == barDeletePoint) return true;

    for (int i = 1; i <= MAX_CIRCLES; i++)
    {
        if (placedCircles.contains(i) && placedCircles[i] == point) return true;
    }
    return false;
}

// Returns the string that contains the given point
int ChordDiagram::getStringNumber(QPointF point)
{
    return std::trunc(static_cast<int>(point.x()) / cellWidth);
}

// Returns the fret that contains the given point
int ChordDiagram::getFretNumber(QPointF point)
{
    return std::trunc(static_cast<int>(point.y()) / cellHeight);
}

// Closes the given string
void ChordDiagram::setStringVisibility(int stringNum, bool visible)
{
    QPushButton *string = stringButtons[stringNum];

    if (!visible)
    {
        if (string->isChecked()) string->click();
        string->setChecked(true);
    }
    else
    {
        string->setText("");
        string->setChecked(false);
    }
}

// Resets the chord diagram
void ChordDiagram::resetDiagram()
{
    placedCircles.clear();
    limitReached = false;

    for (int i = 0; i < 6; i++)
    {
        setStringVisibility(i, true);
    }
    update();
}
