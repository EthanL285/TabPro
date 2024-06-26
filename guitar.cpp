#include "guitar.h"
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>
#include <QPainterPath>
#include <QToolTip>

Guitar::Guitar(QWidget *parent)
    : QWidget{parent}
{
    // Main Layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    mainLayout->setContentsMargins(0, 0, 50, 50);   // Right margin is 50 since spacing in mainWidget is 50

    // View Area Layout (excludes sidebar)
    QVBoxLayout *viewArea = new QVBoxLayout();
    mainLayout->addLayout(viewArea);

    // Tab Layout (includes both tablature and playback)
    tabLayout = new QVBoxLayout();
    tabLayout->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    tabLayout->setSpacing(20);
    viewArea->addLayout(tabLayout);

    // Add tablature and playback buttons
    tab = new Tablature();
    tabLayout->addWidget(tab);

    createPlaybackButtons();

    // Interface layout (includes both playing techniques and guitar interface)
    QVBoxLayout *interfaceLayout = new QVBoxLayout();
    interfaceLayout->setSpacing(0);
    viewArea->addLayout(interfaceLayout);

    // Add Playing Technique Buttons
    playingTechniques = new QWidget();
    playingTechniques->setMaximumHeight(60);
    playingTechniques->setMinimumWidth(1250);
    playingTechniques->setStyleSheet("background-color: rgb(27,27,27); border-top-left-radius: 15px; border-top-right-radius: 15px;");
    interfaceLayout->addWidget(playingTechniques);
    createPlayingTechniqueButtons();

    // Guitar Layout (Includes only the guitar interface)
    rectangle = new Rectangle(1250, 310);
    interfaceLayout->addWidget(rectangle);

    guitarLayout = new QHBoxLayout(rectangle);
    guitarLayout->setContentsMargins(0, 0, 0, 0);
    guitarLayout->setAlignment(Qt::AlignLeft);

    // Create strings
    QWidget *strings = new QWidget();
    strings->setMinimumWidth(50);
    strings->setStyleSheet("background-color: rgb(23,23,23);");
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
}

// Creates buttons for playback
void Guitar::createPlaybackButtons()
{
    QHBoxLayout *playbackLayout = new QHBoxLayout();
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
        "image: url(:/Playback/Icons/Playback/left chevron.png);"
        "background-color: transparent;"
        "border: none;"
        "}"
        "QPushButton:hover {"
        "image: url(:/Playback/Icons/Playback/left chevron hover.png)"
        "}"
    );
    playbackLayout->addWidget(left);
    connect(left, &QPushButton::clicked, tab, &Tablature::goLeft);

    // Play button
    QPushButton *play = new QPushButton();
    play->setFixedSize(31, 31);
    play->setCursor(Qt::PointingHandCursor);
    play->setToolTip("Play the tablature");
    play->setStyleSheet
    (
        "QPushButton {"
        "image: url(:/Playback/Icons/Playback/play.png);"
        "background-color: transparent;"
        "border: none;"
        "}"
        "QPushButton:hover {"
        "image: url(:/Playback/Icons/Playback/play hover.png)"
        "}"
    );
    playbackLayout->addWidget(play);
    connect(play, &QPushButton::clicked, tab, &Tablature::play);

    // Go right button
    QPushButton *right = new QPushButton();
    right->setFixedSize(26, 26);
    right->setCursor(Qt::PointingHandCursor);
    right->setToolTip("Move selection to the right");
    right->setStyleSheet
    (
        "QPushButton {"
        "image: url(:/Playback/Icons/Playback/right chevron.png);"
        "background-color: transparent;"
        "border: none;"
        "}"
        "QPushButton:hover {"
        "image: url(:/Playback/Icons/Playback/right chevron hover.png)"
        "}"
    );
    playbackLayout->addWidget(right);
    connect(right, &QPushButton::clicked, tab, &Tablature::goRight);
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
        fretBoardLayout->addWidget(line);
    }
}

// Creates buttons for inserting playing techniques into tab
void Guitar::createPlayingTechniqueButtons()
{
    QGridLayout *buttonLayout = new QGridLayout(playingTechniques);
    QString symbols[] = {"/", "\\", "h", "p", "b", "r", "x", "~", "-", "|"};
    QString name[] = {"Slide Up", "Slide Down", "Hammer On", "Pull Off", "Bend", "Release", "Muted Hit", "Vibrato", "Rest", "Bar Line"};

    for (int col = 0; col < 10; col++)
    {
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
            "}"
            "QPushButton:hover { "
            "   background-color: rgb(75,75,75);"
            "}"
            "QPushButton:pressed { "
            "   background-color: rgb(25,25,25);"
            "}"
        );
        buttonLayout->addWidget(button, 0, col);
    }
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
        }
    }
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
    shadowEffect->setBlurRadius(10);
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
