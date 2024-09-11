#include "guitar.h"
#include "staff.h"
#include "chordwindow.h"

#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>
#include <QPainterPath>
#include <QToolTip>
#include <QShortcut>
#include <QKeySequence>
#include <QLineEdit>
#include <QIntValidator>
#include <QScrollArea>
#include <QScrollBar>

Guitar::Guitar(QWidget *parent)
    : QWidget{parent}
{
    // Main Layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // View Area Layout (excludes sidebar)
    QVBoxLayout *viewArea = new QVBoxLayout();

    // Add scroll area
    QScrollArea *scrollArea = createScrollArea();
    scrollArea->setWidget(new QWidget());
    scrollArea->widget()->setLayout(viewArea);
    mainLayout->addWidget(scrollArea, Qt::AlignCenter);

    // Add Note line
    QVBoxLayout *noteLayout = new QVBoxLayout();
    noteLayout->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    viewArea->addLayout(noteLayout);

    // QSpacerItem *vSpacer1 = new QSpacerItem(0, 50, QSizePolicy::Fixed, QSizePolicy::Fixed);
    // noteLayout->addItem(vSpacer1);

    Staff *staff = new Staff(this);
    noteLayout->addWidget(staff);

    // QSpacerItem *vSpacer2 = new QSpacerItem(0, 25, QSizePolicy::Fixed, QSizePolicy::Fixed);
    // noteLayout->addItem(vSpacer2);

    // Tab Layout (includes both tablature and playback)
    tabLayout = new QVBoxLayout();
    tabLayout->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    tabLayout->setSpacing(20);
    viewArea->addLayout(tabLayout);

    // Initialise sound object
    sound = new Sound(this);

    // Add tablature and playback buttons
    tab = new Tablature(sound, staff);
    tab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    tab->setContentsMargins(28, 0, 28, 0);
    tabLayout->addWidget(tab);
    createPlaybackButtons();

    // Interface layout (Includes everything below tab)
    QVBoxLayout *interfaceLayout = new QVBoxLayout();
    interfaceLayout->setContentsMargins(0, 0, 0, 50);
    interfaceLayout->setSpacing(0);
    interfaceLayout->setAlignment(Qt::AlignHCenter);
    viewArea->addLayout(interfaceLayout);

    // Bottom Layout (Includes techniques, guitar interface and chords)
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setAlignment(Qt::AlignCenter);
    bottomLayout->setContentsMargins(0,0,0,0);
    bottomLayout->setSpacing(0);
    interfaceLayout->addLayout(bottomLayout);

    // Input Layout (Includes techniques and guitar interface)
    QVBoxLayout *inputLayout = new QVBoxLayout();
    inputLayout->setAlignment(Qt::AlignCenter);
    bottomLayout->addLayout(inputLayout);

    // Add Playing Technique Buttons
    playingTechniques = new QWidget();
    playingTechniques->setFixedSize(1250, 60);
    playingTechniques->setStyleSheet
    (
        "background-color: rgb(23,23,23); "
        "border-top-left-radius: 15px;"
        "border-left: 1px solid rgb(20,20,20);"
        "border-top: 1px solid rgb(20,20,20);"
    );
    inputLayout->addWidget(playingTechniques);
    createPlayingTechniqueButtons();

    // Guitar Layout (Includes only the guitar interface)
    rectangle = new Rectangle(1250, 310);
    inputLayout->addWidget(rectangle);
    guitarLayout = new QHBoxLayout(rectangle);
    guitarLayout->setContentsMargins(0, 0, 0, 0);
    guitarLayout->setAlignment(Qt::AlignLeft);

    // Create strings
    QWidget *strings = new QWidget();
    strings->setMinimumWidth(50);
    strings->setStyleSheet("background-color: rgb(23,23,23); border-left: 1px solid rgb(20,20,20);");
    strings->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    guitarLayout->addWidget(strings);

    // String Layout
    QVBoxLayout *stringLayout = new QVBoxLayout(strings);
    stringLayout->setAlignment(Qt::AlignHCenter);
    stringLayout->setSpacing(22);

    QString stringNames[] = {"E", "B", "G", "D", "A", "E"};
    QLabel* stringLabels[6];

    for (int i = 0; i < 6; ++i)
    {
        stringLabels[i] = createStrings(stringNames[i]);
        stringLayout->addWidget(stringLabels[i], Qt::AlignHCenter);
    }

    // Create fretboard and fret buttons
    createFretBoard();
    createFretButtons();

    // Add chord section
    ChordWindow *chords = new ChordWindow(this);
    bottomLayout->addWidget(chords);
}

// Creates buttons for playback
void Guitar::createPlaybackButtons()
{
    QHBoxLayout *playbackLayout = new QHBoxLayout();
    playbackLayout->setContentsMargins(0, 0, 0, 20);
    playbackLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    tabLayout->addLayout(playbackLayout);

    // Go left button
    QPushButton *left = new QPushButton();
    left->setFixedSize(26, 26);
    left->setCursor(Qt::PointingHandCursor);
    left->setToolTip("Move selection to the left");
    left->setStyleSheet
    (
        "QPushButton {"
        "   image: url(:/Playback/Icons/Playback/left chevron.png);"
        "   background-color: transparent;"
        "   border: none;"
        "   outline: none;"
        "}"
        "QPushButton:hover {"
        "   image: url(:/Playback/Icons/Playback/left chevron hover.png)"
        "}"
    );
    playbackLayout->addWidget(left);
    connect(left, &QPushButton::clicked, tab, &Tablature::goLeft);

    QShortcut *leftShortcut = new QShortcut(Qt::Key_Left, this);
    connect(leftShortcut, &QShortcut::activated, left, &QPushButton::click);

    // Play button
    QPushButton *play = new QPushButton();
    play->setCheckable(true);
    play->setFixedSize(31, 31);
    play->setCursor(Qt::PointingHandCursor);
    play->setToolTip("Play the tablature");
    play->setStyleSheet
    (
        "QPushButton {"
        "   image: url(:/Playback/Icons/Playback/play.png);"
        "   background-color: transparent;"
        "   border: none;"
        "   outline: none;"
        "}"
        "QPushButton:hover {"
        "   image: url(:/Playback/Icons/Playback/play hover.png)"
        "}"
        "QPushButton:checked {"
        "    image: url(:/Playback/Icons/Playback/pause.png);"
        "}"
        "QPushButton:checked:hover {"
        "    image: url(:/Playback/Icons/Playback/pause hover.png);"
        "}"
    );
    playbackLayout->addWidget(play);
    connect(play, &QPushButton::clicked, tab, [=]()
    {
        tab->playTab(play);
    });

    // Go right button
    QPushButton *right = new QPushButton();
    right->setFixedSize(26, 26);
    right->setCursor(Qt::PointingHandCursor);
    right->setToolTip("Move selection to the right");
    right->setStyleSheet
    (
        "QPushButton {"
        "   image: url(:/Playback/Icons/Playback/right chevron.png);"
        "   background-color: transparent;"
        "   border: none;"
        "   outline: none;"
        "}"
        "QPushButton:hover {"
        "   image: url(:/Playback/Icons/Playback/right chevron hover.png)"
        "}"
    );
    playbackLayout->addWidget(right);
    connect(right, &QPushButton::clicked, tab, &Tablature::goRight);

    QShortcut *rightShortcut = new QShortcut(Qt::Key_Right, this);
    connect(rightShortcut, &QShortcut::activated, right, &QPushButton::click);
}

// Labels the guitar string with its corresponding note
QLabel* Guitar::createStrings(QString note)
{
    QLabel *string = new QLabel(note);
    string->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    string->setStyleSheet("color: white; font: 15pt 'Muli';");
    return string;
}

// Creates the fretboard
void Guitar::createFretBoard()
{
    QWidget *fretBoard = new QWidget(rectangle);
    fretBoard->setStyleSheet("background-color: rgb(45,45,45);");
    fretBoard->setFixedSize(1200, 310);
    fretBoard->move(50, 0);

    QVBoxLayout *fretBoardLayout = new QVBoxLayout(fretBoard);
    fretBoardLayout->setContentsMargins(0, 6, 0, 6);
    fretBoardLayout->setSpacing(20);

    for (int i = 0; i < 6; ++i)
    {
        QFrame *line = new QFrame();
        line->setFixedWidth(1220);
        line->setLineWidth(5);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line->setStyleSheet("background-color: gray;");
        fretBoardLayout->addWidget(line);
    }
}

// Creates buttons for inserting playing techniques into tab
void Guitar::createPlayingTechniqueButtons()
{
    QGridLayout *buttonLayout = new QGridLayout(playingTechniques);
    buttonLayout->setAlignment(Qt::AlignHCenter);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(42);

    QString symbols[] = {"/", "\\", "h", "p", "b", "r", "TEMPO", "x", "~", "\u2015", "|", QChar(8634), "\u2715"};
    QString name[] =
    {   "Slide Up (/)", "Slide Down (\\)", "Hammer On (h)", "Pull Off (p)", "Bend (b)", "Release (r)",
        "TEMPO", "Muted Hit (x)", "Vibrato (v)", "Rest (-)", "Bar Line (Shift + \\) ", "Undo (Ctrl + z)", "Remove Notes (Backspace)"
    };

    // Assign each button a name and symbol
    for (int col = 0; col < 13; col++)
    {
        // Create tempo button
        if (col == 6)
        {
            QWidget *tempoButton = createTempoButton();
            buttonLayout->addWidget(tempoButton, 0, col, Qt::AlignVCenter);
            continue;
        }

        QPushButton *button = new QPushButton(QString("%1").arg(symbols[col]));
        button->setToolTip(name[col]);
        button->setFixedSize(40, 40);
        button->setCursor(Qt::PointingHandCursor);
        button->setStyleSheet
        (
            "QPushButton { "
            "   border-radius: 5px;"
            "   border: 0.5px solid rgb(25,25,25);"
            "   background-color: rgb(45,45,45);"
            "   font-size: 16px;"
            "   font-weight: bold;"
            "   outline: none;"
            "}"
            "QPushButton:hover { "
            "   background-color: rgb(75,75,75);"
            "}"
            "QPushButton:pressed { "
            "   background-color: rgb(25,25,25);"
            "}"
        );
        buttonLayout->addWidget(button, 0, col);

        // Assign each button to a function
        connect(button, &QPushButton::clicked, this, [=]()
        {
            switch (col)
            {
                case 0: tab->insertSlideUp(); break;
                case 1: tab->insertSlideDown(); break;
                case 2: tab->insertHammerOn(); break;
                case 3: tab->insertPullOff(); break;
                case 4: tab->insertBend(); break;
                case 5: tab->insertRelease(); break;
                case 6: break;
                case 7: tab->insertMutedHit(); break;
                case 8: tab->insertVibrato(); break;
                case 9: tab->insertRest(); break;
                case 10: tab->insertBarLine(); break;
                case 11: tab->undo(); break;
                case 12: tab->remove(); break;
            }
        });
        // Assign each button to a shortcut
        QKeySequence keySequence;
        switch (col)
        {
            case 0: keySequence = QKeySequence(Qt::Key_Slash); break;
            case 1: keySequence = QKeySequence(Qt::Key_Backslash); break;
            case 2: keySequence = QKeySequence("h"); break;
            case 3: keySequence = QKeySequence("p"); break;
            case 4: keySequence = QKeySequence("b"); break;
            case 5: keySequence = QKeySequence("r"); break;
            case 6: break;
            case 7: keySequence = QKeySequence("x"); break;
            case 8: keySequence = QKeySequence("v"); break;
            case 9: keySequence = QKeySequence("-"); break;
            case 10: keySequence = QKeySequence(Qt::Key_Bar); break;
            case 11: keySequence = QKeySequence(Qt::CTRL | Qt::Key_Z);
                {
                    QString newStyleSheet = button->styleSheet();
                    newStyleSheet.replace("font-size: 16px", "font-size: 20px");
                    button->setStyleSheet(newStyleSheet);
                }
                break;
            case 12: keySequence = QKeySequence(Qt::Key_Backspace); break;
        }
        QShortcut *keyShortcut = new QShortcut(keySequence, this);
        connect(keyShortcut, &QShortcut::activated, button, &QPushButton::click);
    }
}

// Creates the tempo button for changing BPM
QWidget *Guitar::createTempoButton()
{
    QWidget *tempoWidget = new QWidget();
    tempoWidget->setContentsMargins(0, 0, 0, 0);
    tempoWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout *tempoLayout = new QHBoxLayout(tempoWidget);
    tempoLayout->setContentsMargins(0, 0, 0, 0);
    tempoLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    tempoLayout->setSpacing(0);

    // Decrease button
    QPushButton *decrease = new QPushButton("-");
    decrease->setFixedSize(40, 40);
    decrease->setToolTip("Decrease BPM");
    decrease->setCursor(Qt::PointingHandCursor);
    decrease->setStyleSheet
    (
        "QPushButton { "
        "   border-top-left-radius: 5px;"
        "   border-bottom-left-radius: 5px;"
        "   border-top-right-radius: 0px;"
        "   border-bottom-right-radius: 0px;"
        "   background-color: rgb(20,20,20);"
        "   font-size: 22px;"
        "   font-weight: bold;"
        "   text-align: center;"
        "   border: none;"
        "   padding-bottom: 5px;"
        "}"
        "QPushButton:hover { "
        "   background-color: rgb(75,75,75);"
        "}"
        "QPushButton:pressed { "
        "   background-color: rgb(25,25,25);"
        "}"
    );

    // Edit field
    QLineEdit *field = new QLineEdit();
    field->setText("60");
    field->setAlignment(Qt::AlignHCenter);
    field->setFixedSize(80, 40);
    field->setFocusPolicy(Qt::ClickFocus);
    field->setStyleSheet
    (
        "background: rgb(45,45,45);"
        "border: none;"
        "color: white;"
        "font: 15pt Muli;"
        "font-weight: bold;"
        "border-radius: 0px;"
        "padding-left: 2px;"
    );
    field->setMaxLength(3);

    // Increase button
    QPushButton *increase = new QPushButton("+");
    increase->setFixedSize(40, 40);
    increase->setToolTip("Increase BPM");
    increase->setCursor(Qt::PointingHandCursor);
    increase->setStyleSheet
    (
        "QPushButton { "
        "   border-top-left-radius: 0px;"
        "   border-bottom-left-radius: 0px;"
        "   border-top-right-radius: 5px;"
        "   border-bottom-right-radius: 5px;"
        "   background-color: rgb(20,20,20);"
        "   font-size: 22px;"
        "   font-weight: bold;"
        "   text-align: center;"
        "   border: none;"
        "   padding-bottom: 5px;"
        "}"
        "QPushButton:hover { "
        "   background-color: rgb(75,75,75);"
        "}"
        "QPushButton:pressed { "
        "   background-color: rgb(25,25,25);"
        "}"
    );

    // Add widgets to layout
    tempoLayout->addWidget(decrease);
    tempoLayout->addWidget(field);
    tempoLayout->addWidget(increase);

    // Connect buttons to slots
    auto changeTempoButton = [=]()
    {
        tab->changeTempoButton(field, increase, decrease);
    };
    auto changeTempoEdit = [=]()
    {
        tab->changeTempoEdit(field, increase, decrease);
    };
    connect(increase, &QPushButton::clicked, tab, changeTempoButton);
    connect(decrease, &QPushButton::clicked, tab, changeTempoButton);
    connect(field, &QLineEdit::editingFinished, tab, changeTempoEdit);

    return tempoWidget;
}

// Creates buttons that serve as frets
void Guitar::createFretButtons()
{
    QGridLayout *fretButtonLayout = new QGridLayout();
    fretButtonLayout->setHorizontalSpacing(7);
    fretButtonLayout->setContentsMargins(7, 12, 7, 12);
    guitarLayout->addLayout(fretButtonLayout);

    for (int row = 0; row < 6; ++row)
    {
        for (int col = 0; col < 25; ++col)
        {
            QPushButton *button = new QPushButton(QString("%1").arg(col));
            button->setObjectName(QString("%1 %2").arg(row).arg(col));
            button->setFixedSize(40, 40);
            button->setCursor(Qt::PointingHandCursor);
            button->setStyleSheet
            (
                "QPushButton { "
                "   border-radius: 20px;"
                "   background-color: rgb(33,33,33);"
                "   font-size: 14px;"
                "   outline: none;"
                "}"
                "QPushButton:hover { "
                "   background-color: rgb(75,75,75);"
                "}"
                "QPushButton:pressed { "
                "   background-color: rgb(25,25,25);"
                "}"
            );
            fretButtonLayout->addWidget(button, row, col);
            connect(button, &QPushButton::clicked, tab, &Tablature::addFretNumber);
            connect(button, &QPushButton::clicked, this, [=]()
            {
                sound->playNote(button->objectName());
            });
        }
    }
}

// Adds a chord to the tab
void Guitar::addChord(QVector<int> chord)
{
    tab->addChord(chord);
}

// Creates the scroll area for the entire viewing area
QScrollArea *Guitar::createScrollArea()
{
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->viewport()->setStyleSheet
    (
        "background: qradialgradient(cx: 0.5, cy: 0, radius: 0.7, "
        "fx: 0.5, fy: 0, stop: 0 rgb(50,50,50), stop: 1 rgb(25,25,25));"
    );
    scrollArea->setStyleSheet
    (
        ////////// Vertical Stylesheet //////////
        "QScrollBar:vertical {" // Vertical scrollbar
        "    border: 1px solid #222222;"
        "    background: #333333;"
        "    width: 15px;"
        "    margin: 22px 0 22px 0;"
        "}"
        "QScrollBar::handle:vertical {" // Scrollbar handle
        "    background: rgb(80,80,80);"
        "    min-height: 20px;"
        "    border: 1px solid rgb(70,70,70);"
        "    border-top: 1px solid rgb(40,40,40);"
        "    border-bottom: 1px solid rgb(40,40,40);"
        "    border-radius: 1px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: rgb(100,100,100);"
        "}"
        "QScrollBar::handle:vertical:pressed {"
        "    background: rgb(85,85,85);"
        "}"
        "QScrollBar::add-line:vertical {" // Buttons at the end of the scrollbar
        "    border-top: 1px solid rgb(15,15,15);"
        "    border-right: 1px solid rgb(15,15,15);"
        "    background: rgb(25,25,25);"
        "    height: 20px;"
        "    subcontrol-position: bottom;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::sub-line:vertical {"
        "    border-bottom: 1px solid rgb(15,15,15);"
        "    border-right: 1px solid rgb(15,15,15);"
        "    background: rgb(25,25,25);"
        "    height: 20px;"
        "    subcontrol-position: top;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::add-line:vertical:hover, QScrollBar::sub-line:vertical:hover {"
        "    background: rgb(80,80,80);"
        "}"
        "QScrollBar::add-line:vertical:pressed, QScrollBar::sub-line:vertical:pressed {"
        "    background: rgb(40,40,40);"
        "}"
        "QScrollBar::up-arrow:vertical {" // Arrows of the end buttons
        "    width: 8px;"
        "    height: 8px;"
        "    image: url(:/Scroll/Icons/Scroll/scroll up.png);"
        "}"
        "QScrollBar::down-arrow:vertical {"
        "    width: 8px;"
        "    height: 8px;"
        "    image: url(:/Scroll/Icons/Scroll/scroll down.png);"
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {" // Regions above and below the handle
        "    background: none;"
        ////////// Horizontal Stylesheet //////////
        "}"
        "QScrollBar:horizontal {" // Horizontal scrollbar
        "    border: 1px solid #222222;"
        "    background: #333333;"
        "    height: 15px;"
        "    margin: 0 22px 0 22px;"
        "}"
        "QScrollBar::handle:horizontal {" // Scrollbar handle
        "    background: rgb(80,80,80);"
        "    min-width: 20px;"
        "    border: 1px solid rgb(70,70,70);"
        "    border-right: 1px solid rgb(40,40,40);"
        "    border-left: 1px solid rgb(40,40,40);"
        "    border-radius: 1px;"
        "}"
        "QScrollBar::handle:horizontal:hover {"
        "    background: rgb(100,100,100);"
        "}"
        "QScrollBar::handle:horizontal:pressed {"
        "    background: rgb(85,85,85);"
        "}"
        "QScrollBar::add-line:horizontal {" // Buttons at the end of the scrollbar
        "    border-right: 1px solid rgb(15,15,15);"
        "    border-bottom: 1px solid rgb(15,15,15);"
        "    background: rgb(25,25,25);"
        "    width: 20px;"
        "    subcontrol-position: right;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::sub-line:horizontal {"
        "    border-left: 1px solid rgb(15,15,15);"
        "    border-bottom: 1px solid rgb(15,15,15);"
        "    background: rgb(25,25,25);"
        "    width: 20px;"
        "    subcontrol-position: left;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::add-line:horizontal:hover, QScrollBar::sub-line:horizontal:hover {"
        "    background: rgb(80,80,80);"
        "}"
        "QScrollBar::add-line:horizontal:pressed, QScrollBar::sub-line:horizontal:pressed {"
        "    background: rgb(40,40,40);"
        "}"
        "QScrollBar::left-arrow:horizontal {" // Arrows of the end buttons
        "    width: 8px;"
        "    height: 8px;"
        "    image: url(:/Scroll/Icons/Scroll/scroll left.png);"
        "}"
        "QScrollBar::right-arrow:horizontal {"
        "    width: 8px;"
        "    height: 8px;"
        "    image: url(:/Scroll/Icons/Scroll/scroll right.png);"
        "}"
        "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {" // Regions to the left and right of the handle
        "    background: none;"
        "}"
    );
    return scrollArea;
}

Rectangle::Rectangle(int width, int height, QWidget *parent)
    : QWidget(parent), width(width), height(height)
{
    // Set widget size
    setMinimumSize(width, height);
    setMaximumSize(width, height);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    // Create the drop shadow effect
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(5);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    shadowEffect->setOffset(0, 5);
    setGraphicsEffect(shadowEffect);
}

void Rectangle::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    // Set the background color
    painter.setRenderHint(QPainter::Antialiasing);
    QColor color(45,45,45);
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);

    int rectWidth = width;
    int rectHeight = height;
    // int radius = 15;

    // Draw rectangle
    painter.drawRoundedRect(0, 0, rectWidth, rectHeight, 0, 0);
}
