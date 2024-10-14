#ifndef QUAVER_H
#define QUAVER_H

#include <QWidget>

class Quaver : public QWidget
{
    Q_OBJECT
public:
    explicit Quaver(QWidget *parent = nullptr);

    /*
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
*/
};


#endif // QUAVER_H


