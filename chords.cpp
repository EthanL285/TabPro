#include "chords.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsEffect>
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

    QWidget *toggleSwitch = new ToggleSwitch(QColor(45,45,45));
    QLineEdit *searchField = new Field("Chord Finder", false, 150);
    headerLayout->addWidget(toggleSwitch);
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
        "   background: rgb(45,45,45);"
        "   border-left: 1px solid rgb(20,20,20);"
        "   color: gray;"
        "   font: 25pt Muli;"
        "   padding-bottom: 5px;"
        "}"
        "QPushButton:hover {"
        "   background: rgb(60,60,60);"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background: rgb(50,50,50);"
        "   color: gray;"
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

    // Chord diagram
    chordDiagram = new ChordDiagram();
    chordDiagram->setFixedWidth(250);
    chordDiagram->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    windowLayout->addWidget(chordDiagram);

    // Diagram Options
    QGridLayout *buttonLayout = new QGridLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    windowLayout->addLayout(buttonLayout);

    QLabel *chordName = new QLabel("Chord Name");
    chordName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    chordName->setStyleSheet("color: white; font: 11pt Muli; font-weight: semi-bold; border: none;");
    buttonLayout->addWidget(chordName, 0, 0);

    QLineEdit *nameField = new Field("Name", true);
    buttonLayout->addWidget(nameField, 0, 1);

    // Labels
    QLabel *placeMode = new Label("Place Mode");
    QLabel *dragMode = new Label("Drag Mode");
    QLabel *deleteMode = new Label("Delete Mode");
    buttonLayout->addWidget(placeMode, 1, 0);
    buttonLayout->addWidget(dragMode, 2, 0);
    buttonLayout->addWidget(deleteMode, 3, 0);

    // Toggle switches
    ToggleSwitch *placeSwitch = new ToggleSwitch(QColor(23,23,23));
    ToggleSwitch *dragSwitch = new ToggleSwitch(QColor(23,23,23));
    ToggleSwitch *deleteSwitch = new ToggleSwitch(QColor(23,23,23));
    placeSwitch->setObjectName("place");
    dragSwitch->setObjectName("drag");
    deleteSwitch->setObjectName("delete");
    connect(placeSwitch, &ToggleSwitch::clicked, this, &Chords::toggleMode);
    connect(dragSwitch, &ToggleSwitch::clicked, this, &Chords::toggleMode);
    connect(deleteSwitch, &ToggleSwitch::clicked, this, &Chords::toggleMode);
    buttonLayout->addWidget(placeSwitch, 1, 1, Qt::AlignRight);
    buttonLayout->addWidget(dragSwitch, 2, 1, Qt::AlignRight);
    buttonLayout->addWidget(deleteSwitch, 3, 1, Qt::AlignRight);

    // Add button
    QPushButton *addButton = new QPushButton("Add Chord");
    addButton->setMinimumSize(0, 30);
    addButton->setStyleSheet
    (
        "QPushButton {"
        "   background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(255, 255, 255, 200), stop: 1 rgba(96, 94, 92, 200));"
        "   border: none;"
        "   border-radius: 5px;"
        "   color: black;"
        "   font: 11pt Muli;"
        "   font-weight: semi-bold;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(255, 255, 255, 200), stop: 0.2 rgba(243, 242, 241, 200));"
        "}"
        "QPushButton:pressed {"
        "   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba(32, 31, 30, 200), stop: 0.3 rgba(59, 58, 57, 200));"
        "   color: white;"
        "   font: 11pt Muli;"
        "}"
    );
    addButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    addButton->setCursor(Qt::PointingHandCursor);
    addButton->setFocusPolicy(Qt::NoFocus);
    buttonLayout->addWidget(addButton, 4, 0, 1, 2);
}

// Toggles the corresponding mode
void Chords::toggleMode()
{
    ChordDiagram *diagram = qobject_cast<ChordDiagram*>(chordDiagram);
    ToggleSwitch* senderSwitch = qobject_cast<ToggleSwitch*>(sender());

    QString name = senderSwitch->objectName();
    if (name == "place") diagram->placeMode = !diagram->placeMode;
    else if (name == "drag") diagram->dragMode = !diagram->dragMode;
    else if (name == "delete") diagram->deleteMode = !diagram->deleteMode;
}

//////////////////// Chord Diagram Class ////////////////////

ChordDiagram::ChordDiagram(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *diagramLayout = new QVBoxLayout(this);
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);

    // Open and close string buttons
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
        stringButtons.append(openClose);
        connect(openClose, &QPushButton::clicked, this, [openClose]()
        {
            if (openClose->isChecked()) openClose->setText("X");
            else openClose->setText("");
        });
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

    int rows = 4;
    int cols = 5;
    int paddingLeftRight = 15;
    int paddingTop = 35;
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
                int y = j * cellHeight + paddingTop + (cellHeight / 2);
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
    for (const auto pair : placedCircles)
    {
        painter.save();
        painter.drawEllipse(pair.first, 14, 14);

        QRectF textRect(QPoint(10, 10), QSize(20, 20));
        textRect.moveCenter(pair.first);
        painter.setPen(Qt::white);
        painter.drawText(textRect, Qt::AlignCenter, QString::number(pair.second));
        painter.restore();

        // Change string to closed
        int stringNum = std::trunc(static_cast<int>(pair.first.x()) / cellWidth);
        QPushButton *string = stringButtons[stringNum];
        if (string->isChecked()) string->click();
        string->setChecked(true);
    }

    // Draw the circle while hovering
    if (isHovering && placeMode && !limitReached && !currCirclePos.isNull())
    {
        painter.drawEllipse(currCirclePos, 14, 14);

        QRectF textRect(QPoint(10, 10), QSize(20, 20));
        textRect.moveCenter(currCirclePos);
        painter.setPen(Qt::white);
        painter.drawText(textRect, Qt::AlignCenter, QString::number(getCircleNum()));
    }
}

// Hover event (enable mouse tracking)
void ChordDiagram::mouseMoveEvent(QMouseEvent *event)
{
    currCirclePos = snapToGrid(event->pos());
    if (!snap) currCirclePos = event->pos();

    // Cursor hovers above or below the fretboard
    isHovering = !(currCirclePos.y() < 35 || currCirclePos.y() > height() - 35);
    setCursor(isHovering && placeMode && !limitReached ? Qt::PointingHandCursor : Qt::ArrowCursor);

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
    // Place circle if snapped and in place mode
    if (event->button() == Qt::LeftButton && snap && placeMode && !limitReached)
    {
        int idx = getCircleIndex(currCirclePos);

        // Circle is placed on the same string as an already existing circle
        if (onSameString(currCirclePos))
        {
            int num = getCircleNum();
            placedCircles.remove(idx);
            placedCircles.insert(idx, qMakePair(currCirclePos, num));
        }
        else
        {
            placedCircles.append(qMakePair(currCirclePos, getCircleNum()));
        }
        update();

        // Placed circles exceeds the limit
        limitReached = (placedCircles.size() >= 4);
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
    snap = (minDistance > 13) ? false : true;

    return nearestPoint;
}

// Checks whether a point is on the same string as a placed circle
bool ChordDiagram::onSameString(QPointF point)
{
    for (auto pair : placedCircles)
    {
        if (pair.first.x() == point.x()) return true;
    }
    return false;
}

// Return Priority
// 1. Returns the index of the circle given its coordinates
// 2. Returns the index of the circle on the same string
// 3. Returns -1 if no circle is found that matches cases (1) & (2)
int ChordDiagram::getCircleIndex(QPointF point)
{
    int idx = -1;
    for (int i = 0; i < placedCircles.size(); i++)
    {
        if (placedCircles[i].first == point) return i;
        else if (placedCircles[i].first.x() == point.x()) idx = i;
    }
    return idx;
}

// Returns the next circle number to be placed
int ChordDiagram::getCircleNum()
{
    int n = placedCircles.size() + 1;
    int expectedSum = n * (n + 1) / 2;
    int actualSum = 0;

    for (auto pair : placedCircles)
    {
        actualSum += pair.second;
    }

    int result = expectedSum - actualSum;
    if (result == 0) return placedCircles.size();
    else return result;
}

//////////////////// Toggle Switch Class ////////////////////

ToggleSwitch::ToggleSwitch(QColor background, QWidget *parent)
    : background{background}, QWidget{parent}
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
    QColor backgroundColor = toggled ? QColor(0, 175, 0) : background;
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
    emit clicked();
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

// Returns the toggle state of the switch
bool ToggleSwitch::isToggled()
{
    return toggled;
}

//////////////////// Field Class ////////////////////

Field::Field(QString text, bool dark, int width, QWidget *parent)
    : QLineEdit{parent}
{
    if (width == 0) setMinimumSize(0, 35);
    else setFixedSize(width, 35);

    QString background = (dark) ? "rgba(23,23,23,200)" : "rgba(45,45,45,200)";
    QString stylesheet = QString
    (
        "background: %1;"
        "border: 1px solid rgb(20,20,20);"
        "border-radius: 5px;"
        "color: white;"
        "font: 11pt Muli;"
        "padding-left: 6px;"
    ).arg(background);

    setStyleSheet(stylesheet);
    setPlaceholderText(text);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

//////////////////// Label Class ////////////////////

Label::Label(QString text, QWidget *parent)
    : QLabel{parent}
{
    setText(text);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setStyleSheet("color: white; font: 11pt Muli; font-weight: semi-bold; border: none;");
}



