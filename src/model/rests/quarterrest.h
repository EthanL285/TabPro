#ifndef QUARTERREST_H
#define QUARTERREST_H

#include "rest.h"

#include <QWidget>

class QuarterRest : public QWidget, Rest
{
    Q_OBJECT
public:
    explicit QuarterRest(QWidget *parent = nullptr);
    double getBeatValue() override;

signals:
};

#endif // QUARTERREST_H
