#include "note.h"

#include <QVBoxLayout>
#include <QFrame>

Note::Note(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setFixedWidth(length);

    QFrame *noteLine = new QFrame();
    noteLine->setFrameShape(QFrame::HLine);
    noteLine->setFrameShadow(QFrame::Plain);
    noteLine->setStyleSheet("color: white; border: 0px solid black; border-top: 1.5px solid white;");
    mainLayout->addWidget(noteLine, Qt::AlignCenter);
}

// Updates the length of the note line to be the same as the tab length
void Note::updateLineLength(bool add)
{
    // Add/Subtract the length of a tab column
    length += (add ? 35 : -35);
    setFixedWidth(length);
}
