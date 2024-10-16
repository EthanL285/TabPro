#ifndef CHORDDIAGRAM_H
#define CHORDDIAGRAM_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>

#define ROWS 4
#define COLS 5
#define MAX_CIRCLES 4
#define NUM_STRINGS 6
#define CIRCLE_RADIUS 16
#define BAR_POINT QPointF(-14,-14)
#define INVALID_POINT QPointF(-1,-1)

class ChordDiagram : public QWidget {
    Q_OBJECT

public:
    ChordDiagram(QWidget *parent = nullptr);
    QPointF snapToGrid(const QPointF &pos);
    void convertDiagramToTab();
    void convertTabToDiagram(QVector<QPair<int,int>> tab, int barFret = 0, int barLength = 0);

    void drawBar(QFont font, QPainter &painter);
    void drawPlacedCircles(QPainter &painter);
    void drawHoverCircle(QPainter &painter);
    void drawCircle(QPainter &painter, QPointF center, int circleNum);

    QPointF getCircle(int stringNum);
    int getCircleNumber(QPointF point);
    int getNextCircleNumber();
    int getFretNumber(QPointF point);
    int getSameStringCircle(QPointF point);
    int getStringNumber(QPointF point);
    bool isHoveringCircle(QPointF point);
    void setStringVisibility(int stringNum, bool visible);
    void setSnapPositions();

    void handlePlaceMode();
    void handleDragMode();
    void handleDeleteMode();

    void handleDragPlacement(int circleNum, int sameStringCircle, int duplicateCircle, QPointF newPoint);
    void handleSameStringCircle(int circleNum, int sameStringCircle, QPointF newPoint);
    void handleDuplicateCircle(int circleNum, int sameStringCircle, int duplicateCircle);
    void handleValidPlacement(int circleNum, QPointF newPoint);

    friend class ChordWindow;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public slots:
    void resetDiagram();
    void placeBar();

private:
    int cellHeight;
    int cellWidth;
    int paddingLeftRight;
    int paddingTop;
    int paddingBottom;
    int barPlacement = 0;
    int barSpan = NUM_STRINGS;

    bool isWidgetHovered = false;
    bool isCircleHovered = false;
    bool placeMode = false;
    bool dragMode = false;
    bool deleteMode = false;
    bool limitReached = false;
    bool snap = false;
    bool isPressed = false;
    bool barExists = false;

    QVector<int> tabColumn;
    QVector<QPointF> snapPositions;
    QHash<int, QPointF> placedCircles;
    QVector<QPushButton*> stringButtons;
    QPointF barPoint = BAR_POINT;
    QPointF grabbedCircle = INVALID_POINT;
    QPointF currCirclePos = INVALID_POINT;
    QPointF barDeletePoint = INVALID_POINT;
};

#endif // CHORDDIAGRAM_H
