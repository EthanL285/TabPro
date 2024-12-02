#include "eighthrest.h"

#include <QLabel>
#include <QVBoxLayout>

EighthRest::EighthRest()
{
    setFixedWidth(35);

    QLabel *label = new QLabel();
    label->setText("𝄾");
    label->setStyleSheet("font-size: 65px; padding-top: 16px;");
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->setAlignment(label, Qt::AlignCenter);
}

// Get the beat value of the rest
double EighthRest::getBeatValue()
{
    return 0.5;
}
