#ifndef NOTE_H
#define NOTE_H

#include "notetype.h"

#include <QWidget>
#include <QPainter>

class Note : public QWidget
{
    Q_OBJECT
public:
    QVector<int> getStaffLines();

protected:
    explicit Note(QVector<int> staffLines, QWidget *parent = nullptr);

    NoteType type;
    QPainter painter;
    QVector<int> staffLines;    // The staff lines of note heads
    QVector<int> yPos;          // The positions of note heads
    int stemHeight;

    static constexpr int INVALID_LINE = -999;
    static constexpr int STEM_HEIGHT = 56;
    static constexpr int HEAD_WIDTH = 17;
    static constexpr int HEAD_HEIGHT = 11;
    static constexpr double STAFF_SPACING = 7.5;

    bool isSingleNote();
    bool isFlipped();
    void drawExtraLines(int staffLine);
    void drawSingleStem();
    void drawMultiStem(QVector<int> yPos);

signals:
};

#endif // NOTE_H
