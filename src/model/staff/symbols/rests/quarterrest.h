#ifndef QUARTERREST_H
#define QUARTERREST_H

#include "rest.h"

#include <QWidget>

class QuarterRest : public Rest
{
    Q_OBJECT
public:
    explicit QuarterRest();
    double getBeatValue() override;

signals:
};

#endif // QUARTERREST_H
