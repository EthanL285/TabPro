#ifndef STAFF_H
#define STAFF_H

#include "menubar.h"
#include "rhythmsymbol.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QPixmap>

#define UPDATE_LINE 11
#define STAFF_OFFSET 1

class Tablature;

class Staff : public QWidget
{
    Q_OBJECT
public:
    explicit Staff(MenuBar *menu, Tablature *tab, QWidget *parent = nullptr);
    void setTab(Tablature *tab);
    void updateStaff();
    void updateLineLength(bool add);
    void updateHeight(int height, int line);

    QMap<int, QPair<int, int>> createStringMap();
    QMap<int, QString> createNoteMap();
    QPixmap getNotePixmap(QString note);

    void insertRest(int index, double beat);
    void addNote(QVector<int> fretNumbers, int index, bool isChord = false);
    void replaceNote(int index, int line, RhythmSymbol *symbol);
    void removeNote(int index);
    void insertNote(int index, int line, RhythmSymbol *symbol);
    void toggleChordMode();

    QPair<QVector<RhythmSymbol*>, int> getMeasureInfo(int index);
    bool exceedsMeasure(QVector<RhythmSymbol*> measure);
    double getBeats(QVector<RhythmSymbol*> measure);

    static QVector<RhythmSymbol*> notes;
    static constexpr int INVALID_LINE = -999;
    static constexpr double STAFF_SPACING = 7.5;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QHBoxLayout *mainLayout;

    int length = 95;
    int highestLine = UPDATE_LINE;
    bool chordMode = false;

    QString selectedNote = "crotchet";
    QMap<int, QString> noteMap;
    QMap<int, QPair<int, int>> stringMap;

    MenuBar *menu;
    QVector<int> lines;
    Tablature *tab;

signals:

};

#endif // STAFF_H
