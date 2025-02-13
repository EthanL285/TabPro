#ifndef TABLATURESCROLLAREA_H
#define TABLATURESCROLLAREA_H

#include "tablature.h"

#include <QWidget>
#include <QScrollArea>

class TablatureScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit TablatureScrollArea(Tablature *tab, QWidget *parent = nullptr);

signals:
};

#endif // TABLATURESCROLLAREA_H
