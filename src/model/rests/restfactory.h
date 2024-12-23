#ifndef RESTFACTORY_H
#define RESTFACTORY_H

#include "rest.h"
#include "notetype.h"

#include <QWidget>

class RestFactory : public QWidget
{
    Q_OBJECT
public:
    explicit RestFactory(QWidget *parent = nullptr);
    static Rest *createRest(double beatValue);
    static Rest *createRest(NoteType type);

signals:
};

#endif // RESTFACTORY_H
