#ifndef STAFF_H
#define STAFF_H

#include "menubar.h"
#include "rhythmsymbol.h"
#include "signaturewidget.h"
#include "tablaturebutton.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QPixmap>
#include <QLabel>

class Staff : public QWidget
{
    Q_OBJECT
public:
    explicit Staff(MenuBar *menu, QWidget *parent = nullptr);

    int getBeatsPerMeasure();
    int getBeatUnit();
    const QVector<RhythmSymbol*> &getNotes();

    void updateLength(int delta, int factor = 1);
    void toggleChordMode();

    // Note Operations
    bool replaceNote(int index, RhythmSymbol *symbol, TablatureButton *button);
    void addNote(int index, RhythmSymbol *symbol, TablatureButton *button);
    void removeNote(int index);
    void selectNote(int index);

    static const QVector<int> fretToLines(const QVector<int> &fretNumbers);

    // Test functions
    QWidget *getLayoutItem(int index);
    QHBoxLayout *getLayout();

    static constexpr int LAYOUT_OFFSET = 3;
    static constexpr int BARLINE_HEIGHT = 28;
    static constexpr int DEFAULT_HEIGHT = 185;
    static constexpr int INVALID_LINE = -999;
    static constexpr int LINE_COUNT = 5;
    static constexpr int LINE_SPACING = 15;
    static constexpr double STAFF_SPACING = 7.5;
    const QString EMPTY_COLUMN = "\u2015\n\u2015\n\u2015\n\u2015\n\u2015\n\u2015";

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QHBoxLayout *mainLayout;
    int beatsPerMeasure = 4;
    int beatUnit = 4;
    int length = 185;
    bool chordMode = false;

    static const QMap<int, QString> NOTE_MAP;
    static const QMap<int, QPair<int, int>> STRING_MAP;

    MenuBar *menu;
    SignatureWidget *timeSignature;
    QVector<RhythmSymbol*> notes;
    RhythmSymbol *selectedNote = nullptr;

    // Measure functions
    double getBeats(QVector<RhythmSymbol*> measure);
    bool measureExceeded(QVector<RhythmSymbol*> measure);
    bool handleMeasureExceeded(QVector<RhythmSymbol*> measure, int indexInMeasure, int index);
    QPair<QVector<RhythmSymbol*>, int> getMeasureInfo(int index);

    // Note functions
    void addNoteToLayout(int index, RhythmSymbol *symbol);

signals:
    void removeColumn(int index);
    void addColumn(int index, const QString &text, RhythmSymbol *symbol);

public slots:
    void onTimeSignatureChanged(int beatsPerMeasure, int beatUnit);
    void onNoteWidthChange(int newWidth, int prevWidth);
};

#endif // STAFF_H
