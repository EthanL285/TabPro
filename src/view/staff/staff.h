#ifndef STAFF_H
#define STAFF_H

#include "menubar.h"
#include "note.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QPixmap>

#define UPDATE_LINE 11

class Staff : public QWidget
{
    Q_OBJECT
public:
    explicit Staff(MenuBar *menu, QWidget *parent = nullptr);
    void updateLineLength(bool add);
    void updateHeight(int height, int line);
    QMap<int, QPair<int, int>> createStringMap();
    QMap<int, QString> createNoteMap();
    QPixmap getNotePixmap(QString note);
    void addNote(QVector<int> fretNumbers, int index, bool isChord = false);
    void removeNote(int index);
    void addBlank(int index);
    void toggleChordMode();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QHBoxLayout *mainLayout;
    int length = 95;
    int highestLine = UPDATE_LINE;
    bool chordMode = false;
    QString selectedNote = "crotchet";
    QMap<QString, QPixmap> notePixmaps;
    QMap<int, QString> noteMap;
    QMap<int, QPair<int, int>> stringMap;

    MenuBar *menu;
    QVector<Note*> notes;
    QVector<int> lines;

signals:

};

#endif // STAFF_H
