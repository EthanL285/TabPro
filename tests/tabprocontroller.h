#ifndef TABPROCONTROLLER_H
#define TABPROCONTROLLER_H

#include "containerwidget.h"
#include "notetype.h"

#include <QObject>

#define OFFSET 1

class TabProController : public QObject
{
    Q_OBJECT
public:
    explicit TabProController(QObject *parent = nullptr);
    ~TabProController();

    const QVector<RhythmSymbol*> &getNotes();
    QWidget *getStaffItem(int index);
    QWidget *getTabItem(int index);

    void createTab(QString tab);
    void clearTab();
    void verifyTab(QString expectedTab);
    void verifyStaff(QString expectedTab);

    void addNote(int string, int fret, int x);
    void removeNote(int x);
    void changeSelectedNote(NoteType type);
    void moveLeft(int x);
    void moveRight(int x);

private:
    ContainerWidget *widget;

signals:
};

#endif // TABPROCONTROLLER_H
