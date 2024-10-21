#ifndef BLANK_H
#define BLANK_H

#include "note.h"

#include <QWidget>

class Blank : public Note
{
    Q_OBJECT
public:
    explicit Blank(QWidget *parent = nullptr);
};

#endif // BLANK_H
