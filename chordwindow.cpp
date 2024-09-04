#include "chordwindow.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsEffect>
#include <QMouseEvent>
#include <QComboBox>

#define MAX_CIRCLES 4
#define ROWS 4
#define COLS 5

ChordWindow::ChordWindow(QWidget *parent)
    : QWidget{parent}
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
    content->setMinimumWidth(0);
    content->setStyleSheet("background: rgb(45,45,45); border: none;");
    content->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    boxLayout->addWidget(content);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(5);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    shadowEffect->setOffset(0, 4);
    setGraphicsEffect(shadowEffect);

    // Accordion Toggle Button
    accordionToggle = new QPushButton();
    accordionToggle->setCheckable(true);
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
    expandIcon = QIcon(":/Playback/Icons/Playback/right chevron.png");
    collapseIcon = QIcon(":/Playback/Icons/Playback/left chevron.png");

    QPixmap pixmap = expandIcon.pixmap(QSize(16, 16));
    accordionToggle->setIcon(QIcon(pixmap));
    accordionToggle->setIconSize(QSize(16, 16));

    //////////////////// Chord List Window - Header layout ////////////////////

    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setAlignment(Qt::AlignLeft);

    // Chord Mode Switch and Search Field
    QLabel *chordMode = createLabel("Chord Mode");
    QWidget *toggleSwitch = new ToggleSwitch(QColor(45,45,45));
    searchField = createField("Chord Finder", false, 150);
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

    QVBoxLayout *listLayout = new QVBoxLayout(chordList);
    listLayout->setContentsMargins(0,0,0,0);
    listLayout->setSpacing(0);
    listLayout->setAlignment(Qt::AlignTop);

    // New Chord Button
    QPushButton *newChord= new QPushButton("+");
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
    listLayout->addWidget(newChord);
    connect(newChord, &QPushButton::clicked, this, &ChordWindow::changeWindow);

    // Chord Item Buttons
    for (int i = 0; i < 20; i++)
    {
        QWidget *chord = new QWidget();
        chord->setFixedHeight(50);
        chord->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        listLayout->addWidget(chord);
    }

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

    QGridLayout *buttonLayout = new QGridLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setContentsMargins(10, 0, 10, 0);
    windowLayout->addLayout(buttonLayout);

    // Chord Name Field
    QLabel *chordName = new QLabel("Chord Name");
    chordName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    chordName->setStyleSheet("color: white; font: 11pt Muli; font-weight: semi-bold; border: none;");
    buttonLayout->addWidget(chordName, 0, 0);

    QLineEdit *nameField = createField("Name", true);
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
    connect(addButton, &QPushButton::clicked, this, &ChordWindow::addChord);

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
        "   image: url(:/Scroll/Icons/Scroll/scroll down.png);"
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
    trash->setFixedSize(21,21);
    trash->setCursor(Qt::PointingHandCursor);
    trash->setVisible(false);
    trash->setToolTip("Reset Chord");
    trash->setStyleSheet
    (
        "QPushButton {"
        "   image: url(:/Miscellaneous/Icons/Miscellaneous/trash.png);"
        "   background-color: transparent;"
        "   border: none;"
        "   outline: none;"
        "}"
        "QPushButton:hover {"
        "   image: url(:/Miscellaneous/Icons/Miscellaneous/trash hover.png)"
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
        "   image: url(:/Miscellaneous/Icons/Miscellaneous/return.png);"
        "   background-color: transparent;"
        "   border: none;"
        "   outline: none;"
        "}"
        "QPushButton:hover {"
        "   image: url(:/Miscellaneous/Icons/Miscellaneous/return hover.png)"
        "}"
    );
    headerLayout->addWidget(back);
    connect(back, &QPushButton::clicked, this, &ChordWindow::changeWindow);
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

    update();
}

// Adds a new chord to the list
void ChordWindow::addChord()
{
    qDebug() << "Added Chord";
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
