#include "chords.h"

#include <QHBoxLayout>

Chords::Chords(QWidget *parent)
    : QWidget{parent}
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Add box
    content = new QWidget();
    content->setFixedWidth(500);
    content->setStyleSheet("background: red;");
    content->setVisible(false);
    content->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    mainLayout->addWidget(content);

    // Add button
    button = new QPushButton(">>");
    button->setCheckable(true);
    button->setFixedWidth(20);
    button->setCursor(Qt::PointingHandCursor);
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    button->setStyleSheet
    (
        "QPushButton {"
        "background: rgb(17,17,17);"
        "border: none;"
        "color: white;"
        "font: 15pt Moon;"
        "font-weight: semi-bold;"
        "}"
        "QPushButton:hover {"
        "background: rgb(22,22,22);"
        "border: 1px solid rgb(17,17,17);"
        "}"
        "QPushButton:pressed {"
        "background: rgb(70,70,70);"
        "font: 15pt Moon;"
        "}"
    );
    mainLayout->addWidget(button);

    connect(button, &QPushButton::toggled, this, &Chords::toggleContent);
}

// Toggles the chord content
void Chords::toggleContent(bool checked)
{
    if (checked && !contentVisible)
    {
        contentVisible = true;
        content->setVisible(true);
        button->setText("<<");
    }
    else if (!checked && contentVisible)
    {
        contentVisible = false;
        content->setVisible(false);
        button->setText(">>");
    }
}

