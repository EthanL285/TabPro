#include "quarterrest.h"

#include <QLabel>
#include <QVBoxLayout>

QuarterRest::QuarterRest()
{
    setFixedWidth(35);

    QLabel *label = new QLabel();
    label->setText("𝄽");
    label->setStyleSheet("font-size: 58px; padding-bottom: 8px;");
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->setAlignment(label, Qt::AlignCenter);
}

// Get the beat value of the rest
double QuarterRest::getBeatValue()
{
    return 1;
}
