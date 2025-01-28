#ifndef NOTE_H
#define NOTE_H

#include "rhythmsymbol.h"
#include "notetype.h"

#include <QWidget>
#include <QPainter>

class Note : public RhythmSymbol
{
    Q_OBJECT
public:
    QVector<int> getStaffLines();
    NoteType getType();
    void toggleSelect() override;

protected:
    explicit Note(QVector<int> staffLines);

    NoteType type;
    QPainter painter;
    QVector<int> staffLines;    // The staff lines of note heads
    QVector<int> yPos;          // The positions of note heads
    int stemHeight;

    static constexpr int STEM_HEIGHT = 56;
    static constexpr int HEAD_WIDTH = 17;
    static constexpr int HEAD_HEIGHT = 11;

    bool isSingleNote();
    bool isFlipped();
    void drawNoteHead(QPainter &painter);
    void drawExtraLines(int staffLine);
    void drawSingleStem();
    void drawMultiStem(QVector<int> yPos);

private:
    bool isSelected = false;

signals:
};

#endif // NOTE_H
