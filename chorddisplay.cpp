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
        "   border: 1px solid rgb(70,70,70);"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgb(70,70,70);"
        "}"
    );
}

// Context menu event
void ChordDisplay::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    menu.setStyleSheet
    (
        "QMenu {"
        "    background-color: rgb(18,18,18);"
        "    border: 1px solid rgb(45,45,45);"
        "}"
        "QMenu::item {"
        "    padding: 5px;"
        "    padding-left: 10px;"
        "    padding-right: 5px;"
        "    background: transparent;"
        "}"
        "QMenu::item:selected {"
        "    color: rgb(237, 67, 55);"
        "}"
        "QMenu::item:pressed {"
        "    background: rgb(30,30,30);"
        "    color: rgb(237, 67, 55);"
        "    border-radius: 5px;"
        "}"
        "QMenu::icon {"
        "    padding-left: 20px;"
        "}"
    );
    QAction* del = menu.addAction(QIcon(":/Miscellaneous/Icons/Miscellaneous/trash.png"), "Delete");
    connect(del, &QAction::triggered, this, [this]() { emit deleted(); });
    connect(&menu, &QMenu::aboutToHide, this, &ChordDisplay::unHoverButton);
    menu.exec(event->globalPos());
}

// Fixes the visual bug where a button remains in the hover state after opening
// another context menu
void ChordDisplay::unHoverButton()
{
    setAttribute(Qt::WA_UnderMouse, false);
}
