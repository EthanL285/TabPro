#ifndef WHOLEREST_H
#define WHOLEREST_H

#include "rest.h"

#include <QWidget>

class WholeRest : public QWidget, Rest
{
    Q_OBJECT
public:
    explicit WholeRest(QWidget *parent = nullptr);
    double getBeatValue() override;

signals:
};

#endif // WHOLEREST_H
