#include "chordwindow.h"
#include "chorddisplay.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsEffect>
#include <QMouseEvent>
#include <QComboBox>
#include <QFile>
#include <QMessageBox>

#define MAX_CIRCLES 4
#define ROWS 4
#define COLS 5

ChordWindow::ChordWindow(ContainerWidget *parent)
    : guitar{parent}, QWidget{parent}
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QVBoxLayout *boxLayout = new QVBoxLayout();
    boxLayout->setSpacing(0);
    boxLayout->setContentsMargins(0,0,0,0);
    boxLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(boxLayout);

    // Box header
    header = new QWidget();
    header->setFixedHeight(60);
    header->setMaximumWidth(0);
    header->setMinimumWidth(0);
    header->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    header->setStyleSheet
    (
        "background: rgb(23,23,23); "
        "border-top: 1px solid rgb(20,20,20); "
        "border-left: 1px solid rgb(20,20,20);"
    );
    boxLayout->addWidget(header);

    // Box Content
    content = new QWidget();
    content->setFixedHeight(310);
    content->setMinimumWidth(0);
    content->setStyleSheet("background: rgb(45,45,45); border: none;");
    content->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    boxLayout->addWidget(content);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(5);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    shadowEffect->setOffset(0, 5);
    setGraphicsEffect(shadowEffect);

    // Accordion Toggle Button
    accordionToggle = new QPushButton();
    accordionToggle->setCheckable(true);
    accordionToggle->setFixedHeight(370);
    accordionToggle->setFixedWidth(20);
    accordionToggle->setFocusPolicy(Qt::NoFocus);
    accordionToggle->setCursor(Qt::PointingHandCursor);
    accordionToggle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    connect(accordionToggle, &QPushButton::toggled, this, &ChordWindow::toggleContent);
    accordionToggle->setStyleSheet
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
    mainLayout->addWidget(accordionToggle);

    // Accordion Toggle Button Icon
    expandIcon = QIcon(":/playback/playback/right chevron.png");
    collapseIcon = QIcon(":/playback/playback/left chevron.png");

    QPixmap pixmap = expandIcon.pixmap(QSize(16, 16));
    accordionToggle->setIcon(QIcon(pixmap));
    accordionToggle->setIconSize(QSize(16, 16));

    //////////////////// Chord List Window - Header layout ////////////////////

    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setAlignment(Qt::AlignLeft);

    // Chord Mode Switch and Search Field
    QLabel *chordMode = createLabel("Chord Mode");
    ToggleSwitch *toggleSwitch = new ToggleSwitch(QColor(45,45,45));
    searchField = createField("Chord Finder", false, 150);
    connect(toggleSwitch, &ToggleSwitch::clicked, this, &ChordWindow::toggleChordMode);
    connect(searchField, &QLineEdit::textChanged, this, &ChordWindow::searchChord);
    headerLayout->addWidget(chordMode);
    headerLayout->addWidget(toggleSwitch);
    headerLayout->addWidget(searchField);

    //////////////////// Chord List Window - Content layout ////////////////////

    QVBoxLayout *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setAlignment(Qt::AlignLeft);

    // Stacked Widget
    stackedWidget = new QStackedWidget();
    stackedWidget->setMinimumWidth(0);
    stackedWidget->setMaximumWidth(0);
    contentLayout->addWidget(stackedWidget);

    // Chord List
    QWidget *chordList = new QWidget();
    chordList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chordList->setStyleSheet("background: rgb(33,33,33); border-left: 1px solid rgb(20,20,20);");

    scrollArea = new QScrollArea();
    scrollArea->setWidget(chordList);
    scrollArea->setWidgetResizable(true);
    stackedWidget->addWidget(scrollArea);

    //////////////////// Chord List Window - List layout ////////////////////

    QVBoxLayout *listContainer = new QVBoxLayout(chordList);
    listContainer->setContentsMargins(0,0,0,0);
    listContainer->setSpacing(0);
    listContainer->setAlignment(Qt::AlignTop);

    listLayout = new QGridLayout();
    listLayout->setContentsMargins(1,0,1,0);
    listLayout->setSpacing(2);
    listContainer->addLayout(listLayout);

    // New Chord Button
    QPushButton *newChord = new QPushButton("+");
    newChord->setFixedHeight(40);
    newChord->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    newChord->setCursor(Qt::PointingHandCursor);
    newChord->setFocusPolicy(Qt::NoFocus);
    newChord->setToolTip("Add New Chord");
    newChord->setStyleSheet
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
    listContainer->addStretch();
    listContainer->addWidget(newChord, Qt::AlignBottom);
    connect(newChord, &QPushButton::clicked, this, &ChordWindow::changeWindow);

    //////////////////// Chord Diagram Window - Content Layout ////////////////////

    chordWindow = new QWidget();
    chordWindow->setStyleSheet("background: rgb(33,33,33); border-left: 1px solid rgb(20,20,20);");
    chordWindow->setContentsMargins(5, 0, 0, 0);
    stackedWidget->addWidget(chordWindow);

    QHBoxLayout *windowLayout = new QHBoxLayout(chordWindow);
    windowLayout->setAlignment(Qt::AlignLeft);

    // Chord Diagram
    diagram = new ChordDiagram();
    windowLayout->addWidget(diagram);

    buttonLayout = new QGridLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setContentsMargins(10, 0, 10, 0);
    windowLayout->addLayout(buttonLayout);

    // Chord Name Field
    QLabel *chordName = new QLabel("Chord Name");
    chordName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    chordName->setStyleSheet("color: white; font: 11pt Muli; font-weight: semi-bold; border: none;");
    buttonLayout->addWidget(chordName, 0, 0);

    nameField = createField("Name", true);
    buttonLayout->addWidget(nameField, 0, 1);

    // Labels
    QLabel *placeMode = createLabel("Place Mode");
    QLabel *dragMode = createLabel("Drag Mode");
    QLabel *deleteMode = createLabel("Delete Mode");
    buttonLayout->addWidget(placeMode, 1, 0);
    buttonLayout->addWidget(dragMode, 2, 0);
    buttonLayout->addWidget(deleteMode, 3, 0);

    // Toggle switches
    placeSwitch = new ToggleSwitch(QColor(23,23,23));
    dragSwitch = new ToggleSwitch(QColor(23,23,23));
    deleteSwitch = new ToggleSwitch(QColor(23,23,23));
    placeSwitch->setObjectName("place");
    dragSwitch->setObjectName("drag");
    deleteSwitch->setObjectName("delete");
    connect(placeSwitch, &ToggleSwitch::clicked, this, &ChordWindow::toggleMode);
    connect(dragSwitch, &ToggleSwitch::clicked, this, &ChordWindow::toggleMode);
    connect(deleteSwitch, &ToggleSwitch::clicked, this, &ChordWindow::toggleMode);
    buttonLayout->addWidget(placeSwitch, 1, 1, Qt::AlignRight);
    buttonLayout->addWidget(dragSwitch, 2, 1, Qt::AlignRight);
    buttonLayout->addWidget(deleteSwitch, 3, 1, Qt::AlignRight);

    // Add Chord button
    QPushButton *addButton = new QPushButton("Add Chord");
    addButton->setMinimumSize(0, 30);
    addButton->setStyleSheet
    (
        "QPushButton {"
        "   background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(255, 255, 255, 200), stop: 1 rgba(96, 94, 92, 200));"
        "   border: 1px solid rgb(20,20,20);"
        "   border-radius: 5px;"
        "   color: black;"
        "   font: 11pt Muli;"
        "   font-weight: semi-bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 rgba(255, 255, 255, 200), stop: 0.2 rgba(243, 242, 241, 200));"
        "}"
        "QPushButton:pressed {"
        "   background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba(32, 31, 30, 200), stop: 0.3 rgba(59, 58, 57, 200));"
        "   color: white;"
        "   font: 11pt Muli;"
        "}"
    );
    addButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    addButton->setCursor(Qt::PointingHandCursor);
    addButton->setFocusPolicy(Qt::NoFocus);
    buttonLayout->addWidget(addButton, 5, 0, 1, 2);
    connect(addButton, &QPushButton::clicked, this, &ChordWindow::addChordToList);

    //////////////////// Chord Diagram Window - Header Layout ////////////////////

    // Bar Dropdown
    barPlacement = createLabel("Bar Placement");
    barDropdown = new QComboBox();
    barDropdown->setFixedSize(100, 32);
    barDropdown->setCursor(Qt::PointingHandCursor);
    barDropdown->addItem("None");
    barDropdown->setVisible(false);
    barPlacement->setVisible(false);
    barDropdown->setStyleSheet
    (
        "QComboBox {"
        "   background-color: rgb(45,45,45);"
        "   border: 1px solid rgb(20,20,20);"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   font: 10pt Muli;"
        "}"
        "QComboBox::drop-down {"
        "   background-color: rgb(17,17,17);"
        "   width: 25px;"
        "}"
        "QComboBox::down-arrow {"
        "   width: 10px;"
        "   height: 10px;"
        "   image: url(:/scroll/scroll/scroll down.png);"
        "   subcontrol-position: center;"
        "}"
        "QComboBox QAbstractItemView {"
        "   selection-background-color: #d0d0d0;"
        "   selection-color: #000;"
        "}"
    );
    for (int i = 1; i < 22; i++)
    {
        QString fret = QString("Fret %1").arg(i);
        barDropdown->addItem(fret);
    }
    connect(barDropdown, &QComboBox::currentTextChanged, diagram, &ChordDiagram::placeBar);

    headerLayout->addWidget(barPlacement);
    headerLayout->addWidget(barDropdown);
    headerLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Reset Chord Button
    trash = new QPushButton();
    trash->setFixedSize(22,22);
    trash->setCursor(Qt::PointingHandCursor);
    trash->setVisible(false);
    trash->setToolTip("Reset Chord");
    trash->setStyleSheet
    (
        "QPushButton {"
        "   image: url(:/miscellaneous/miscellaneous/trash.png);"
        "   background-color: transparent;"
        "   border: none;"
        "   padding: 1px 1px;"
        "}"
        "QPushButton:hover {"
        "   image: url(:/miscellaneous/miscellaneous/trash hover.png)"
        "}"
    );
    QObject::connect(trash, &QPushButton::clicked, this, [&]()
    {
        barDropdown->setCurrentIndex(0);
        diagram->resetDiagram();
    });
    headerLayout->addWidget(trash);
    headerLayout->addItem(new QSpacerItem(8, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));

    // Back Button
    back = new QPushButton();
    back->setFixedSize(25,25);
    back->setVisible(false);
    back->setToolTip("Go Back");
    back->setCursor(Qt::PointingHandCursor);
    back->setStyleSheet
    (
        "QPushButton {"
        "   image: url(:/miscellaneous/miscellaneous/return.png);"
        "   background-color: transparent;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   image: url(:/miscellaneous/miscellaneous/return hover.png)"
        "}"
    );
    headerLayout->addWidget(back);
    connect(back, &QPushButton::clicked, this, &ChordWindow::changeWindow);

    // Default chords
    addDefaultChords();
}

// Fixes the visual bug where the edges of the chord button would remain in the
// hover state after being clicked
bool ChordWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Enter || event->type() == QEvent::Leave) {
        QWidget *button = qobject_cast<QWidget*>(obj);
        if (button) updateScrollArea();
    }
    return QObject::eventFilter(obj, event);
}

// Adds default chords to the list
void ChordWindow::addDefaultChords()
{
    // Read chords from file
    QFile file(":/default/defaultchords.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Could not open chords file for reading");
        return;
    }
    // Process each line/chord
    QTextStream in(&file);
    while (!in.atEnd())
    {
        int barPlacement = 0;
        int barSpan = 0;

        QString line = in.readLine();
        QStringList parts = line.split(";");

        QString chordName = parts[0];
        QStringList fretPos = parts[1].split(",");
        QVector<QPair<int, int>> chord;

        for (const QString &string : fretPos)
        {
            QStringList posParts = string.split(":");
            int fret = posParts[0].toInt();
            int finger = posParts[1].toInt();
            chord.append(qMakePair(fret, finger));
        }
        // Bar chord
        if (parts.size() == 3)
        {
            QStringList bar = parts[2].split(",");
            barPlacement = bar[0].toInt();
            barSpan = bar[1].toInt();
        }
        diagram->convertTabToDiagram(chord, barPlacement, barSpan);
        nameField->setText(chordName);
        addChordToList();
    }
    diagram->resetDiagram();
}

// Searches for the inputted chord
void ChordWindow::searchChord(QString input)
{
    QVector<QPushButton*> hidden;
    for (QPushButton *chord : chordDisplays)
    {
        if (!chord->objectName().contains(input, Qt::CaseInsensitive)) hidden.append(chord);
    }
    updateLayout(hidden);
}

// Updates the grid layout to fill available space
void ChordWindow::updateLayout(QVector<QPushButton*> hidden)
{
    // Re-add visible widgets
    int count = 0;
    for (QPushButton *chord : chordDisplays)
    {
        int row = count / 3;
        int col = count % 3;
        if (!hidden.contains(chord))
        {
            listLayout->addWidget(chord, row, col);
            chord->setVisible(true);
            count++;
        }
        else
        {
            chord->setVisible(false);
        }
    }
}

// Toggles the chord content
void ChordWindow::toggleContent()
{
    // Handle animation
    animateAccordion(header);
    animateAccordion(content);
    animateAccordion(stackedWidget);
    contentToggled = !contentToggled;

    // Change button icon
    if (contentToggled) accordionToggle->setIcon(collapseIcon);
    else accordionToggle->setIcon(expandIcon);
}

// Animates the accordion
void ChordWindow::animateAccordion(QWidget *widget)
{
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "minimumWidth");
    int endWidth = (contentToggled) ? 0 : 530;

    animation->setStartValue(widget->minimumWidth());
    animation->setEndValue(endWidth);
    animation->setDuration(250);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

// Changes the chord content window
void ChordWindow::changeWindow()
{
    bool isFirstWindow = stackedWidget->currentWidget() == scrollArea;
    int idx = (isFirstWindow) ? 1 : 0;
    stackedWidget->setCurrentIndex(idx);

    searchField->setVisible(!isFirstWindow);
    back->setVisible(isFirstWindow);
    trash->setVisible(isFirstWindow);
    barPlacement->setVisible(isFirstWindow);
    barDropdown->setVisible(isFirstWindow);

    if (statusMessage) removeStatusMessage();
    update();
}

// Removes the status message
void ChordWindow::removeStatusMessage()
{
    statusMessage->setVisible(false);
}

// Adds a new chord to the list
void ChordWindow::addChordToList()
{
    Status status = Status::Success;
    QString chordName = nameField->text();

    // Validate chord name
    if (chordName.isEmpty()) status = Status::Empty;
    else if (chordName.size() > 12) status = Status::CharLimit;
    else if (chords.contains(chordName)) status = Status::Duplicate;

    addStatusMessage(status);
    if (status != Status::Success) return;

    // Add new chord to the list
    ChordDisplay *chordItem = new ChordDisplay(diagram, nameField->text());
    chordItem->setObjectName(chordName);
    connect(chordItem, &QPushButton::clicked, this, &ChordWindow::addChordToStaff);
    connect(chordItem, &ChordDisplay::deleted, this, &ChordWindow::deleteChord);
    connect(chordItem, &QPushButton::released, this, &ChordWindow::updateScrollArea);
    connect(chordItem, &QPushButton::pressed, this, &ChordWindow::updateScrollArea);
    chordItem->installEventFilter(this);

    int chordCount = chordDisplays.size();
    int row = chordCount / 3;
    int col = chordCount % 3;
    listLayout->addWidget(chordItem, row, col);

    nameField->clear();
    diagram->resetDiagram();
    barDropdown->setCurrentIndex(0);
    chordDisplays.append(chordItem);
    chords.insert(chordName, diagram->tabColumn);
}

// Repaints the scroll area
void ChordWindow::updateScrollArea()
{
    scrollArea->viewport()->update();
}

// Deletes the chord that emitted the signal
void ChordWindow::deleteChord()
{
    ChordDisplay *chord = qobject_cast<ChordDisplay*>(sender());
    chordDisplays.removeAll(chord);
    chord->deleteLater();
    updateLayout();
}

// Adds a status message upon clicking new chord
void ChordWindow::addStatusMessage(Status status)
{
    QString message;
    QString colour = "rgb(237, 67, 55)";
    switch (status)
    {
        case Status::Success:
            message = QString::fromUtf8("\u2713 ") + "Chord successfully added";
            colour = "rgb(114, 191, 106)";
            break;
        case Status::Empty:
            message = QString::fromUtf8("\u2717 ") + "Chord name is empty";
            break;
        case Status::Duplicate:
            message = QString::fromUtf8("\u2717 ") + "Chord name already exists";
            break;
        case Status::CharLimit:
            message = QString::fromUtf8("\u2717 ") + "Character limit exceeded";
            break;
    }
    QString stylesheet = QString("color: %1; font: 11pt Muli; border: none;").arg(colour);

    // Add status message
    if (!statusMessage)
    {
        spacer = new QSpacerItem(0, 5, QSizePolicy::Expanding, QSizePolicy::Fixed);
        statusMessage = new QLabel(message);
        buttonLayout->addItem(spacer, 6, 0, 1, 2);
        buttonLayout->addWidget(statusMessage, 7, 0, 1, 2, Qt::AlignCenter);
    }
    statusMessage->setVisible(true);
    statusMessage->setText(message);
    statusMessage->setStyleSheet(stylesheet);
}

// Adds the selected chord to the staff
void ChordWindow::addChordToStaff()
{
    QPushButton *chord = qobject_cast<QPushButton*>(sender());
    guitar->addChord(chords[chord->objectName()]);
}

// Toggles chord mode
void ChordWindow::toggleChordMode()
{
    guitar->toggleChordMode();
}

// Toggles the corresponding mode
void ChordWindow::toggleMode()
{
    ToggleSwitch* senderSwitch = qobject_cast<ToggleSwitch*>(sender());

    // Toggle the given mode
    Mode currentMode = getModeFromName(senderSwitch->objectName());
    switch (currentMode)
    {
        case Place: diagram->placeMode = !diagram->placeMode; break;
        case Drag: diagram->dragMode = !diagram->dragMode; break;
        case Delete: diagram->deleteMode = !diagram->deleteMode; break;
        default: break;
    }
    // Reset other switches
    if (currentMode != Place) resetSwitch(placeSwitch, diagram->placeMode);
    if (currentMode != Drag) resetSwitch(dragSwitch, diagram->dragMode);
    if (currentMode != Delete) resetSwitch(deleteSwitch, diagram->deleteMode);
}

// Returns the mode given a name
Mode ChordWindow::getModeFromName(QString name)
{
    if (name == "place") return Place;
    if (name == "drag") return Drag;
    if (name == "delete") return Delete;
    return None;
}

// Resets the given switch
void ChordWindow::resetSwitch(ToggleSwitch *widget, bool &mode)
{
    if (widget->isToggled())
    {
        mode = false;
        widget->toggle();
    }
}

// Creates a new line edit
QLineEdit *ChordWindow::createField(QString text, bool dark, int width)
{
    QLineEdit *field = new QLineEdit();

    if (width == 0) field->setMinimumSize(0, 33);
    else field->setFixedSize(width, 33);

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

    field->setStyleSheet(stylesheet);
    field->setPlaceholderText(text);
    field->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    return field;
}

// Creates a new label
QLabel *ChordWindow::createLabel(QString text)
{
    QLabel *label = new QLabel(text);
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    label->setStyleSheet("color: white; font: 11pt Muli; font-weight: semi-bold; border: none;");

    return label;
}
