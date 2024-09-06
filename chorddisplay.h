#ifndef CHORDDISPLAY_H
#define CHORDDISPLAY_H

#include "chorddiagram.h"

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPropertyAnimation>

class ChordDisplay : public QPushButton
{
    Q_OBJECT
public:
    explicit ChordDisplay(ChordDiagram *diagram, QString name, QWidget *parent = nullptr);

signals:
};

#endif // CHORDDISPLAY_H
