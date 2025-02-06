#ifndef STAFF_H
#define STAFF_H

#include "menubar.h"
#include "rhythmsymbol.h"
#include "signaturewidget.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QPixmap>
#include <QLabel>

#define UPDATE_LINE 11

class Staff : public QWidget
{
    Q_OBJECT
public:
    explicit Staff(MenuBar *menu, QWidget *parent = nullptr);
    int getBeatsPerMeasure();
    int getBeatUnit();
    QWidget *getLayoutItem(int index);
    QHBoxLayout *getLayout();
    const QVector<RhythmSymbol*> &getNotes();

    QMap<int, QPair<int, int>> createStringMap();
    QMap<int, QString> createNoteMap();

    void updateLength(bool increase, int multiplier);
    void updateHeight(int height, int line);

    void insertRest(int index, double beat, bool emitSignal);
    void insertNote(int index, int line, RhythmSymbol *symbol);
    bool addNote(QVector<int> fretNumbers, int index, bool isChord = false);
    void addNoteToLayout(int index, RhythmSymbol *symbol);
    void removeNote(int index, bool emitSignal);
    bool replaceNote(int index, int line, RhythmSymbol *symbol);
    void selectNote(int index);

    void toggleChordMode();

    QPair<QVector<RhythmSymbol*>, int> getMeasureInfo(int index);
    bool exceedsMeasure(QVector<RhythmSymbol*> measure);
    double getBeats(QVector<RhythmSymbol*> measure);

    static constexpr int LAYOUT_OFFSET = 3;
    static constexpr int BARLINE_HEIGHT = 28;
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

    QMap<int, QString> noteMap;
    QMap<int, QPair<int, int>> stringMap;

    MenuBar *menu;
    SignatureWidget *timeSignature;
    QVector<int> lines;
    QVector<RhythmSymbol*> notes;
    RhythmSymbol *selectedNote = nullptr;

signals:
    void noteRemoved(int index);
    void restInserted(int index);

public slots:
    void onNoteRemoved(int index);
    void onTimeSignatureChanged(int beatsPerMeasure, int beatUnit);
};

#endif // STAFF_H
