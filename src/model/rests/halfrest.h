#ifndef HALFREST_H
#define HALFREST_H

#include "rest.h"

#include <QWidget>

class HalfRest : public QWidget, Rest
{
    Q_OBJECT
public:
    explicit HalfRest(QWidget *parent = nullptr);
    double getBeatValue() override;

signals:
};

#endif // HALFREST_H
