#include "menubar.h"
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QDockWidget>
#include <QPushButton>
#include <QLabel>

MenuBar::MenuBar(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create menu bar
    QMenuBar *menuBar = new QMenuBar();
    menuBar->addMenu(tr("&File"));
    menuBar->addMenu(tr("&Edit"));
    menuBar->setStyleSheet("QMenuBar { background-color: rgb(23,23,23); border-bottom: 1px solid rgb(18,18,18); }");
    mainLayout->addWidget(menuBar);

    // Create bottom widget
    QWidget *bottomWidget = new QWidget();
    bottomWidget->setFixedHeight(40);
    bottomWidget->setStyleSheet("background-color: rgb(23,23,23); border-bottom: 1px solid rgb(18,18,18);");
    mainLayout->addWidget(bottomWidget);

    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget);
    bottomLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->setSpacing(0);

    QVector<QString> notes = {"𝅝","𝅗𝅥","𝅘𝅥","𝅘𝅥𝅮","𝅘𝅥𝅯","𝅘𝅥𝅰","𝅘𝅥𝅱"};
    QVector<QString> signs = {"♭","♮","♯"};

    // Add sign buttons
    for (const QString &sign : signs)
    {
        QPushButton *signButton = createButton(sign, 20);
        bottomLayout->addWidget(signButton);
    }
    // Add divider
    QHBoxLayout *divider = createDivider();
    bottomLayout->addLayout(divider);

    // Add note buttons
    for (const QString &note : notes)
    {
        QPushButton *noteButton = createButton(note, 30);
        bottomLayout->addWidget(noteButton);
    }
}

// Creates a menu bar button
QPushButton *MenuBar::createButton(QString text, int fontSize)
{
    QPushButton *button = new QPushButton(text);
    button->setFixedWidth(40);
    button->setCursor(Qt::PointingHandCursor);
    button->setStyleSheet(QString(
        "QPushButton { border: none; font-size: %1px; }"
        "QPushButton:hover { color: rgb(70, 129, 232); }"
    ).arg(fontSize));

    return button;
}

// Creates a menu bar divider
QHBoxLayout *MenuBar::createDivider()
{
    QHBoxLayout *dividerLayout = new QHBoxLayout();
    QSpacerItem *spacer1 = new QSpacerItem(15, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    QSpacerItem *spacer2 = new QSpacerItem(15, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);

    QWidget *divider = new QWidget();
    divider->setFixedSize(1, 25);
    divider->setStyleSheet("background-color: gray;");

    dividerLayout->addSpacerItem(spacer1);
    dividerLayout->addWidget(divider);
    dividerLayout->addSpacerItem(spacer2);

    return dividerLayout;
}


