#ifndef TABFRET_H
#define TABFRET_H

#include "tabstate.h"

#include <QPushButton>
#include <QWidget>

class TabFret : public QPushButton
{
    Q_OBJECT
public:
    explicit TabFret(int string, QWidget *parent = nullptr);

    int getString();
    int getFret();
    void setFret(int fret);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    const int EMPTY_FRET = -1;
    int fret = EMPTY_FRET;
    int string;

    State state = State::Normal;
    QColor textBackground = columnStateColours[State::Normal];

public slots:
    void onToggle(bool checked);
    void onColumnStateChange(State state);

signals:
    void mouseMoved();
    void mouseLeft();
    void keyPressed(int fret, int string);
};

#endif // TABFRET_H
