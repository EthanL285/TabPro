#ifndef TABCOLUMN_H
#define TABCOLUMN_H

#include "tabfret.h"
#include "tabstate.h"

#include <QWidget>
#include <QPushButton>

class TabColumn : public QWidget
{
    Q_OBJECT
public:
    explicit TabColumn(QWidget *parent = nullptr);

    const QVector<TabFret*> &getButtons();
    QVector<int> getFrets();
    void setFrets(const QVector<int> &frets);

    int getString();
    void select(int string);
    void deselect();

    bool isEmpty();
    bool isSelected();
    QJsonArray toJson() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    State state = State::Normal;
    QVector<TabFret*> buttons;

private slots:
    void onHover();
    void onLeave();
    void onClick();

signals:
    void keyPressed(int fret, int string);
    void stateChanged(State state);
};

#endif // TABCOLUMN_H
