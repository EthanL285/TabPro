#ifndef TABPROCONTROLLER_H
#define TABPROCONTROLLER_H

#include "containerwidget.h"
#include "notetype.h"
#include "menubar.h"

#include <QObject>

class TabProController : public QObject
{
    Q_OBJECT
public:
    explicit TabProController(QObject *parent = nullptr);
    ~TabProController();
    QVector<RhythmSymbol*> getNotes();

    void addNote(int string, int fret);
    void removeNote();
    void changeSelectedNote(NoteType type);

private:
    ContainerWidget *widget;
    MenuBar *menu;

signals:
};

#endif // TABPROCONTROLLER_H
