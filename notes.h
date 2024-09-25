#ifndef NOTES_H
#define NOTES_H

#include <QWidget>

// ========== Blank Class ==========
class Blank : public QWidget
{
    Q_OBJECT
public:
    explicit Blank(QWidget *parent = nullptr);
};


// ========== Crotchet Class ==========
class Crotchet : public QWidget
{
    Q_OBJECT
public:
    explicit Crotchet(QVector<int> staffLines, QWidget *parent = nullptr);
    bool isSingleNote();
    QVector<int> getStaffLines();
    void drawAdditionalLines(QPainter &painter, int staffLine);
    void drawSingleStem(QPainter &painter);
    void drawChordStem(QPainter &painter, QVector<int> yPos);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<int> staffLines;
    bool flip;
    bool addLines;
};

#endif // NOTES_H
