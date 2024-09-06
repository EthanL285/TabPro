#include "chorddisplay.h"

#define SCALE_FACTOR 2

ChordDisplay::ChordDisplay(ChordDiagram *diagram, QString name, QWidget *parent)
    : QPushButton{parent}
{
    QVBoxLayout *displayLayout = new QVBoxLayout(this);
    displayLayout->setAlignment(Qt::AlignCenter);

    // Render and display an image of the chord diagram
    QPixmap pixmap = QPixmap(diagram->size() * SCALE_FACTOR);
    pixmap.setDevicePixelRatio(SCALE_FACTOR);
    diagram->render(&pixmap);
    pixmap = pixmap.scaled(QSize(250,250), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QLabel *diagramDisplay= new QLabel();
    diagramDisplay->setPixmap(pixmap);
    diagramDisplay->setStyleSheet("border: none;");

    QLabel *chordName = new QLabel(name);
    chordName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    chordName->setStyleSheet("background-color: none; color: white; font: 11pt Muli; border: none;");

    displayLayout->addWidget(diagramDisplay);
    displayLayout->addWidget(chordName);
    displayLayout->setAlignment(chordName, Qt::AlignCenter);

    setFixedHeight(180);
    setCursor(Qt::PointingHandCursor);
    setFocusPolicy(Qt::NoFocus);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setStyleSheet
    (
        "QPushButton {"
        "   background-color: rgb(33,33,33);"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(40,40,40);"
        "   border: none;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgb(70,70,70);"
        "   font: 15pt Moon;"
        "}"
    );
}
