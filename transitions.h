#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include <QWidget>
#include <QObject>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QStackedWidget>
#include <QMainWindow>

class Transitions : public QObject
{
    Q_OBJECT
public:
    explicit Transitions(QMainWindow *parent = nullptr);
    void fadeInOut(QWidget *fadeInWidget, QWidget *fadeOutWidget, QStackedWidget *stack);
};


#endif // TRANSITIONS_H
