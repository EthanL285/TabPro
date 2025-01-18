#ifndef STAFF_H
#define STAFF_H

#include "menubar.h"
#include "rhythmsymbol.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QPixmap>

#define UPDATE_LINE 11

class Staff : public QWidget
{
    Q_OBJECT
public:
    explicit Staff(MenuBar *menu, QWidget *parent = nullptr);
    int getBeatsPerMeasure();
    int getBeatUnit();

    const QVector<RhythmSymbol*> &getNotes();
    QWidget *getLayoutItem(int index);
    QHBoxLayout *getLayout();

    void updateLineLength(bool add);
    void updateHeight(int height, int line);

    QMap<int, QPair<int, int>> createStringMap();
    QMap<int, QString> createNoteMap();

    void insertRest(int index, double beat, bool emitSignal);
    void addNote(QVector<int> fretNumbers, int index, bool isChord = false);
    void addNoteToLayout(int index, RhythmSymbol *symbol);
    void removeNote(int index, bool emitSignal);
    void replaceNote(int index, int line, RhythmSymbol *symbol);
    void insertNote(int index, int line, RhythmSymbol *symbol);
    void toggleChordMode();

    QPair<QVector<RhythmSymbol*>, int> getMeasureInfo(int index);
    bool exceedsMeasure(QVector<RhythmSymbol*> measure);
    double getBeats(QVector<RhythmSymbol*> measure);

    static constexpr int STAFF_OFFSET = 3;
    static constexpr int INVALID_LINE = -999;
    static constexpr double STAFF_SPACING = 7.5;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QHBoxLayout *mainLayout;
    int beatsPerMeasure = 4;
    int beatUnit = 4;
    int length = 150;
    int highestLine = UPDATE_LINE;
    bool chordMode = false;

    QString selectedNote = "crotchet";
    QMap<int, QString> noteMap;
    QMap<int, QPair<int, int>> stringMap;

    MenuBar *menu;
    QVector<int> lines;
    QVector<RhythmSymbol*> notes;

signals:
    void noteRemoved(int index);
    void restInserted(int index);

public slots:
    void onNoteRemoved(int index);
    void onTimeSignatureChanged(int beatsPerMeasure, int beatUnit);
};

#endif // STAFF_H
