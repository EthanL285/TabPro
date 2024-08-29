#ifndef CHORDS_H
#define CHORDS_H

#include <QWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>

class Chords : public QWidget
{
    Q_OBJECT
public:
    explicit Chords(QWidget *parent = nullptr);
    void toggleContent();
    void animateAccordion(QWidget *widget);
    void toggleMode();

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
    QWidget *chordDiagram;
};

//////////////////// Chord Diagram Class ////////////////////

class ChordDiagram : public QWidget {
    Q_OBJECT

public:
    ChordDiagram(QWidget *parent = nullptr);
    QPointF snapToGrid(const QPointF &pos);
    bool onSameString(QPointF point);
    int getCircleNum();
    int getCircleIndex(QPointF point);
    friend class Chords;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Chords *chords;
    bool isHovering = false;
    bool placeMode = false;
    bool dragMode = false;
    bool deleteMode = false;
    bool limitReached = false;
    bool snap = false;
    bool circlePositionsFound = false;
    QVector<QPointF> circlePositions;
    QVector<QPair<QPointF, int>> placedCircles;
    QPointF currCirclePos = QPointF(-1,-1);
    QVector<QPushButton*> stringButtons;
};

//////////////////// Toggle Switch Class ////////////////////

class ToggleSwitch : public QWidget {
    Q_OBJECT

public:
    ToggleSwitch(QColor background, QWidget *parent = nullptr);
    bool isToggled();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked();

private:
    QColor background;
    QWidget *handle;
    bool toggled = false;

private slots:
    void animateHandle();
};

//////////////////// Field Class ////////////////////

class Field : public QLineEdit {
    Q_OBJECT

public:
    Field(QString text, bool dark, int width = 0, QWidget *parent = nullptr);
};

//////////////////// Label Class ////////////////////

class Label : public QLabel {
    Q_OBJECT

public:
    Label(QString text, QWidget *parent = nullptr);
};

#endif // CHORDS_H
