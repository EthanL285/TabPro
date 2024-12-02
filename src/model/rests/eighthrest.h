#ifndef WHOLEREST_H
#define WHOLEREST_H

#include "rest.h"

#include <QWidget>

class EighthRest : public Rest
{
    Q_OBJECT
public:
    explicit EighthRest();
    double getBeatValue() override;

signals:
};

#endif // WHOLEREST_H
