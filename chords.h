#ifndef CHORDS_H
#define CHORDS_H

#include <QWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>

class Chords : public QWidget
{
    Q_OBJECT
public:
    explicit Chords(QWidget *parent = nullptr);
    void toggleContent();
    void animateAccordion(QWidget *widget);

private slots:
    void addChord();

private:
    QWidget *content;
    QWidget *header;
    QPushButton *button;
    bool contentToggled = false;
    QIcon expandIcon;
    QIcon collapseIcon;
    QScrollArea *scrollArea;
    QStackedWidget *stackedWidget;
};

//////////////////// Chord Diagram Class ////////////////////

class ChordDiagram : public QWidget {
    Q_OBJECT

public:
    ChordDiagram(QWidget *parent = nullptr);
    QPointF snapToGrid(const QPointF &pos);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool isHovering = false;
    bool snap = false;
    bool circlePositionsFound = false;
    QVector<QPointF> circlePositions;
    QVector<QPointF> placedCirclePos;
    QPointF currentCirclePos = QPointF(-1,-1);
};

//////////////////// Toggle Switch Class ////////////////////

class ToggleSwitch : public QWidget {
    Q_OBJECT

public:
    ToggleSwitch(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QWidget *handle;
    bool toggled = false;

private slots:
    void animateHandle();
};


#endif // CHORDS_H
