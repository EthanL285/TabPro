#include "chords.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QPropertyAnimation>
#include <QLabel>
#include <QGraphicsEffect>
#include <QlineEdit>
#include <QMouseEvent>

Chords::Chords(QWidget *parent)
    : QWidget{parent}
{   
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setAlignment(Qt::AlignLeft);

    QVBoxLayout *boxLayout = new QVBoxLayout();
    boxLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(boxLayout);

    // Add box header
    header = new QWidget();
    header->setFixedHeight(60);
    header->setMaximumWidth(0);
    header->setMinimumWidth(0);
    header->setStyleSheet("background: rgb(23,23,23); border-top: 2px solid rgb(20,20,20); border-left: 1px solid rgb(20,20,20);");
    header->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    boxLayout->addWidget(header);

    // Add box
    content = new QWidget();
    content->setFixedHeight(310);
    content->setMinimumWidth(0);
    content->setStyleSheet("background: rgb(45,45,45); border: none;");
    content->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    boxLayout->addWidget(content);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(5);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    shadowEffect->setOffset(0, 4);
    setGraphicsEffect(shadowEffect);

    // Add button
    button = new QPushButton();
    button->setCheckable(true);
    button->setFixedWidth(20);
    button->setCursor(Qt::PointingHandCursor);
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    button->setFocusPolicy(Qt::NoFocus);
    button->setStyleSheet
    (
        "QPushButton {"
        "   background: rgb(17,17,17);"
        "   border: none;"
        "   color: white;"
        "   font: 15pt Moon;"
        "   font-weight: semi-bold;"
        "}"
        "QPushButton:hover {"
        "   background: rgb(22,22,22);"
        "   border: 1px solid rgb(17,17,17);"
        "}"
        "QPushButton:pressed {"
        "   background: rgb(70,70,70);"
        "   font: 15pt Moon;"
        "}"
    );
    mainLayout->addWidget(button);
    connect(button, &QPushButton::toggled, this, &Chords::toggleContent);

    // Set button icon
    expandIcon = QIcon(":/Playback/Icons/Playback/right chevron.png");
    collapseIcon = QIcon(":/Playback/Icons/Playback/left chevron.png");

    QPixmap pixmap = expandIcon.pixmap(QSize(16, 16));
    button->setIcon(QIcon(pixmap));
    button->setIconSize(QSize(16, 16));

    // Header layout
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setAlignment(Qt::AlignLeft);

    // Header section
    QLabel *chordMode = new QLabel("Chord Mode");
    chordMode->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    chordMode->setStyleSheet("color: white; font: 11pt Muli; font-weight: semi-bold; border: none;");
    headerLayout->addWidget(chordMode);

    QWidget *toggleSwitch = new ToggleSwitch();
    headerLayout->addWidget(toggleSwitch);

    QLineEdit *searchField = new QLineEdit(this);
    searchField->setPlaceholderText("Chord Finder");
    searchField->setFixedSize(150, 35);
    searchField->setStyleSheet
    (
        "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(96, 94, 92, 100), stop: 1 rgba(40,40,40, 200));"
        "border: 1px solid rgb(20,20,20);"
        "border-radius: 5px;"
        "color: white;"
        "font: 11pt Muli;"
        "padding-left: 6px;"
    );
    headerLayout->addWidget(searchField);

    // Content layout
    QVBoxLayout *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setAlignment(Qt::AlignLeft);

    // Content section
    stackedWidget = new QStackedWidget();
    stackedWidget->setMinimumWidth(0);
    stackedWidget->setMaximumWidth(0);
    contentLayout->addWidget(stackedWidget);

    QWidget *chordList = new QWidget();
    chordList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chordList->setStyleSheet("background: rgb(33,33,33); border-left: 1px solid rgb(20,20,20);");

    scrollArea = new QScrollArea();
    scrollArea->setWidget(chordList);
    scrollArea->setWidgetResizable(true);
    stackedWidget->addWidget(scrollArea);

    // Chord list section
    QVBoxLayout *listLayout = new QVBoxLayout(chordList);
    listLayout->setContentsMargins(0,0,0,0);
    listLayout->setSpacing(0);
    listLayout->setAlignment(Qt::AlignTop);

    QPushButton *addChord = new QPushButton("+");
    addChord->setFixedHeight(40);
    addChord->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    addChord->setCursor(Qt::PointingHandCursor);
    addChord->setFocusPolicy(Qt::NoFocus);
    addChord->setToolTip("Add New Chord");
    addChord->setStyleSheet
    (
        "QPushButton {"
            "background: rgb(45,45,45);"
            "border-left: 1px solid rgb(20,20,20);"
            "color: gray;"
            "font: 25pt Muli;"
            "padding-bottom: 5px;"
        "}"
        "QPushButton:hover {"
            "background: rgb(60,60,60);"
            "color: white;"
        "}"
        "QPushButton:pressed {"
            "background: rgb(50,50,50);"
            "color: gray;"
        "}"
    );
    listLayout->addWidget(addChord);
    connect(addChord, &QPushButton::clicked, this, &Chords::addChord);

    for (int i = 0; i < 20; i++)
    {
        QWidget *chord = new QWidget();
        chord->setFixedHeight(50);
        // chord->setStyleSheet("background: transparent;");
        chord->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        listLayout->addWidget(chord);
    }
}

// Toggles the chord content
void Chords::toggleContent()
{
    // Handle animation
    animateAccordion(header);
    animateAccordion(content);
    animateAccordion(stackedWidget);
    contentToggled = !contentToggled;

    // Change button icon
    if (contentToggled) button->setIcon(collapseIcon);
    else button->setIcon(expandIcon);
}

// Animates the accordion
void Chords::animateAccordion(QWidget *widget)
{
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "minimumWidth");
    int endWidth = (contentToggled) ? 0 : 500;

    animation->setStartValue(widget->minimumWidth());
    animation->setEndValue(endWidth);
    animation->setDuration(250);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

// Adds a new chord to the list
void Chords::addChord()
{
    QWidget *chordWindow = new QWidget();
    chordWindow->setStyleSheet("background: rgb(33,33,33); border-left: 1px solid rgb(20,20,20);");
    stackedWidget->addWidget(chordWindow);
    stackedWidget->setCurrentWidget(chordWindow);

    QHBoxLayout *windowLayout = new QHBoxLayout(chordWindow);
    windowLayout->setAlignment(Qt::AlignLeft);

    QWidget *chordDiagram = new ChordDiagram();
    chordDiagram->setFixedWidth(250);
    chordDiagram->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    windowLayout->addWidget(chordDiagram);

    QHBoxLayout *stringLayout = new QHBoxLayout(chordDiagram);
    stringLayout->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    stringLayout->setSpacing(34);

    QVector<QString> strings = {"E","A","D","G","B","E"};
    for (int i = 0; i < 6; i++)
    {
        QLabel *letter = new QLabel(strings[i]);
        letter->setStyleSheet("color: white; font: 10pt Muli; border: none;");
        stringLayout->addWidget(letter);
    }

    /* Features to be implemented:
     * Snap buttons to grids when widget is hovered on
     * Set Chord Name
     * Bar Option
     * Open and closed strings
     */
}

//////////////////// Chord Diagram Class ////////////////////

ChordDiagram::ChordDiagram(QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
}

// Paint event
void ChordDiagram::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::white, 0.5));
    painter.save();

    int rows = 4;
    int cols = 5;
    int paddingLeftRight = 15;
    int paddingTop = 30;
    int paddingBottom = 35;
    int cellWidth = (width() - 2 * paddingLeftRight) / cols;
    int cellHeight = (height() - paddingTop - paddingBottom) / rows;

    // Draw horizontal lines
    for (int i = 0; i < rows; i++)
    {
        int y = i * cellHeight + paddingTop;
        if (i == 0)
        {
            painter.setPen(QPen(Qt::white, 5, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine(paddingLeftRight, y, width() - paddingLeftRight, y);
            painter.restore();
        }
        else
        {
            painter.drawLine(paddingLeftRight, y, width() - paddingLeftRight, y);
        }
    }
    // Explicitly draw the bottom line at the bottom of the grid due to rounding errors
    painter.drawLine(paddingLeftRight, height() - paddingBottom, width() - paddingLeftRight, height() - paddingBottom);

    // Draw vertical lines
    for (int i = 0; i <= cols; i++)
    {
        int x = i * cellWidth + paddingLeftRight;
        painter.drawLine(x, paddingTop, x, height() - paddingBottom);

        // Find circle positions
        if (!circlePositionsFound)
        {
            for (int j = 0; j < rows; j++)
            {
                int y = j * cellHeight + (cellHeight);
                circlePositions.append(QPointF(x, y));
            }
        }
    }
    circlePositionsFound = true;

    QFont font("Muli", 10);
    font.setBold(true);
    painter.setFont(font);
    painter.setBrush(QBrush(QColor(80,80,80), Qt::SolidPattern));
    painter.setPen(QPen(QColor(45,45,45), 1));

    // Draw currently placed circles
    for (const auto &point : placedCirclePos)
    {
        painter.save();
        painter.drawEllipse(point, 14, 14);

        QRectF textRect(QPoint(10, 10), QSize(20, 20));
        textRect.moveCenter(point);
        painter.setPen(Qt::white);
        painter.drawText(textRect, Qt::AlignCenter, "1");
        painter.restore();
    }

    // Draw the circle while hovering
    if (isHovering && !currentCirclePos.isNull())
    {
        painter.drawEllipse(currentCirclePos, 14, 14);
    }
}

// Hover event (enable mouse tracking)
void ChordDiagram::mouseMoveEvent(QMouseEvent *event)
{
    currentCirclePos = snapToGrid(event->pos());
    if (!snap) currentCirclePos = event->pos();

    isHovering = true;
    update();
}

// Leave event
void ChordDiagram::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    isHovering = false;
    update();
}

// Press Event
void ChordDiagram::mousePressEvent(QMouseEvent *event)
{
    // Place circle if snapped
    if (event->button() == Qt::LeftButton && snap)
    {
        placedCirclePos.append(currentCirclePos);
        update();
    }
}

// Snaps the circle to a position on the grid
QPointF ChordDiagram::snapToGrid(const QPointF &pos)
{
    // Calculate the shortest euclidean distance
    QPointF nearestPoint;
    double minDistance = std::numeric_limits<double>::max();

    for (const auto &point : circlePositions)
    {
        double distance = std::sqrt(std::pow(point.x() - pos.x(), 2) + std::pow(point.y() - pos.y(), 2));
        if (distance < minDistance)
        {
            minDistance = distance;
            nearestPoint = point;
        }
    }
    // Snap if minimum distance is less than 10
    snap = (minDistance > 13) ? false : true;

    return nearestPoint;
}

//////////////////// Toggle Switch Class ////////////////////

ToggleSwitch::ToggleSwitch(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(60, 40);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("border: none;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(6,0,0,0);
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    handle = new QWidget();
    handle->setFixedSize(24,24);
    handle->setStyleSheet("background: white; border-radius: 12px; border: none;");
    mainLayout->addWidget(handle);
}

// Paint event
void ToggleSwitch::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int rectWidth = 55;
    int rectHeight = 30;
    int x = (width() - rectWidth) / 2;
    int y = (height() - rectHeight) / 2;
    QRect rect(x, y, rectWidth, rectHeight);
    QColor backgroundColor = toggled ? QColor(0, 175, 0) : QColor(45, 45, 45);
    painter.setBrush(QBrush(backgroundColor));
    painter.setPen(QPen(QColor(20,20,20), 1));
    painter.drawRoundedRect(rect, 15,15);

    update();
}

// Mouse press event
void ToggleSwitch::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    animateHandle();
    toggled = !toggled;
}

// Animates the switch handle
void ToggleSwitch::animateHandle()
{
    QPropertyAnimation* animation = new QPropertyAnimation(handle, "pos");
    QPoint startPoint = handle->pos();
    QPoint endPoint = startPoint;

    if (!toggled) endPoint.setX(30);
    else endPoint.setX(6);

    animation->setStartValue(startPoint);
    animation->setEndValue(endPoint);
    animation->setDuration(150);
    animation->setEasingCurve(QEasingCurve::Linear);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}


