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

    // Tab functions
    void createTab(QString tab);
    void clearTab();

    // Verification functions
    void verifyTab(QString expectedTab);
    void verifyStaff(QString expectedTab);

    // Note functions
    void addNote(int string, int fret, int x = 1);
    void removeNote(int x);
    void addChord(QVector<int> fretNumbers);

    // Move functions
    void moveLeft(int x);
    void moveRight(int x);

    // Set functions
    void setSelectedNote(NoteType type);
    void setChordMode(bool enable);

private:
    ContainerWidget *widget;

signals:
};

#endif // TABPROCONTROLLER_H
