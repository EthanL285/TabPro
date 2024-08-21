#include "menubar.h"
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QDockWidget>
#include <QPushButton>

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

    /*
    // Create bottom widget
    QWidget *bottomWidget = new QWidget();
    bottomWidget->setFixedHeight(32);
    bottomWidget->setStyleSheet("background-color: rgb(23,23,23); border-bottom: 1px solid rgb(18,18,18);");
    mainLayout->addWidget(bottomWidget);

    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget);
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->setSpacing(0);

    // Add buttons
    QPushButton *button1 = new QPushButton("Note 1", bottomWidget);
    QPushButton *button2 = new QPushButton("Note 2", bottomWidget);
    button1->setFixedWidth(100);
    button2->setFixedWidth(100);
    button1->setStyleSheet("QPushButton { border: none; }");
    button2->setStyleSheet("QPushButton { border: none; }");
    bottomLayout->addWidget(button1);
    bottomLayout->addWidget(button2); */
}
