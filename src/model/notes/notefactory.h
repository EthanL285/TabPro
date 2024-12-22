#ifndef NOTEFACTORY_H
#define NOTEFACTORY_H

#include "note.h"
#include "notetype.h"

#include <QWidget>

class NoteFactory : public QWidget
{
    Q_OBJECT
public:
    explicit NoteFactory(QWidget *parent = nullptr);
    static Note *createNote(NoteType type, QVector<int> staffLines);

signals:
};

#endif // NOTEFACTORY_H
