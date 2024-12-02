#ifndef HALFREST_H
#define HALFREST_H

#include "rest.h"

#include <QWidget>

class HalfRest : public Rest
{
    Q_OBJECT
public:
    explicit HalfRest();
    double getBeatValue() override;

signals:
};

#endif // HALFREST_H
