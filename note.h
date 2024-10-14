#ifndef NOTE_H
#define NOTE_H

#include <QWidget>
#include <QPainter>

class Note : public QWidget
{
    Q_OBJECT
public:
    explicit Note(QVector<int> staffLines, QWidget *parent = nullptr);
    QVector<int> getStaffLines();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool isSingleNote();
    void drawExtraLines(int staffLine);
    void drawSingleStem();
    void drawMultiStem(QVector<int> yPos);
    void drawFlag(QVector<int> yPos);

    QVector<int> staffLines;    // Contains the staff line location of note heads
    QPainter painter;
    bool addLines;
    bool flip;
signals:
};

#endif // NOTE_H
