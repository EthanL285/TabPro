#ifndef CHORDS_H
#define CHORDS_H

#include <QWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QHash>

#define NUM_STRINGS 6
#define BAR_POINT QPointF(-14,-14)
#define INVALID_POINT QPointF(-1,-1)

enum Mode {
    Place,
    Drag,
    Delete,
    None
};

//////////////////// Toggle Switch Class ////////////////////

class ToggleSwitch : public QWidget {
    Q_OBJECT

public:
    ToggleSwitch(QColor background, QWidget *parent = nullptr);
    bool isToggled();
    void toggle();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked();

private:
    QColor background;
    QWidget *handle;
    bool toggled = false;

public slots:
    void animateHandle();
};

//////////////////// Chords Class /////////////////////

class Chords : public QWidget
{
    Q_OBJECT
public:
    explicit Chords(QWidget *parent = nullptr);
    void toggleContent();
    void animateAccordion(QWidget *widget);
    void toggleMode();
    void resetSwitch(ToggleSwitch *widget, bool &mode);
    Mode getModeFromName(QString name);

private slots:
    void addChord();
    void changeWindow();

private:
    QWidget *content;
    QWidget *header;
    QLineEdit *searchField;
    QPushButton *accordionToggle;
    QPushButton *trash;
    QPushButton *back;
    QLabel *barPlacement;
    QComboBox *barDropdown;
    bool contentToggled = false;
    QIcon expandIcon;
    QIcon collapseIcon;
    QScrollArea *scrollArea;
    QStackedWidget *stackedWidget;
    QWidget *chordWindow = nullptr;
    QWidget *chordDiagram;
    ToggleSwitch *placeSwitch;
    ToggleSwitch *dragSwitch;
    ToggleSwitch *deleteSwitch;
};

//////////////////// Chord Diagram Class ////////////////////

class ChordDiagram : public QWidget {
    Q_OBJECT

public:
    ChordDiagram(QWidget *parent = nullptr);
    QPointF snapToGrid(const QPointF &pos);

    void drawBar(QFont font, QPainter &painter);
    void drawPlacedCircles(QPainter &painter);
    void drawHoverCircle(QPainter &painter);
    void drawCircle(QPainter &painter, QPointF center, int circleNum);

    bool isHoveringCircle(QPointF point);
    int getCircleNumber(QPointF point);
    int getNextCircleNumber();
    int getFretNumber(QPointF point);
    int getSameStringCircle(QPointF point);
    int getStringNumber(QPointF point);
    void setStringVisibility(int stringNum, bool visible);
    void setSnapPositions();

    void handlePlaceMode();
    void handleDragMode();
    void handleDeleteMode();

    void handleDragPlacement(int circleNum, int sameStringCircle, int duplicateCircle, QPointF newPoint);
    void handleSameStringCircle(int circleNum, int sameStringCircle, QPointF newPoint);
    void handleDuplicateCircle(int circleNum, int sameStringCircle, int duplicateCircle);
    void handleValidPlacement(int circleNum, QPointF newPoint);

    friend class Chords;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void resetDiagram();
    void placeBar(QComboBox *dropdown);

private:
    Chords *chords;
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

    QVector<QPointF> snapPositions;
    QHash<int, QPointF> placedCircles;
    QVector<QPushButton*> stringButtons;
    QPointF barPoint = BAR_POINT;
    QPointF grabbedCircle = INVALID_POINT;
    QPointF currCirclePos = INVALID_POINT;
    QPointF barDeletePoint = INVALID_POINT;
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
