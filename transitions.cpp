#include "transitions.h"

Transitions::Transitions(QWidget *parent) : QObject(parent) {}

// Fade transition between two widets
void Transitions::fadeInOut(QWidget *fadeInWidget, QWidget *fadeOutWidget, QStackedWidget *stack)
{
    // Create opacity effects for current and next widgets
    QGraphicsOpacityEffect *currentOpacityEffect = new QGraphicsOpacityEffect(fadeInWidget);
    fadeInWidget->setGraphicsEffect(currentOpacityEffect);

    QGraphicsOpacityEffect *nextOpacityEffect = new QGraphicsOpacityEffect(fadeOutWidget);
    nextOpacityEffect->setOpacity(0);
    fadeOutWidget->setGraphicsEffect(nextOpacityEffect);

    // Fade out current widget
    QPropertyAnimation *fadeOutAnimation = new QPropertyAnimation(currentOpacityEffect, "opacity");
    fadeOutAnimation->setDuration(500);
    fadeOutAnimation->setStartValue(1.0);
    fadeOutAnimation->setEndValue(0.0);
    fadeOutAnimation->setEasingCurve(QEasingCurve::Linear);

    fadeOutAnimation->start();

    // Fade in next widget
    QPropertyAnimation *fadeInAnimation = new QPropertyAnimation(nextOpacityEffect, "opacity");
    fadeInAnimation->setDuration(500);
    fadeInAnimation->setStartValue(0.0);
    fadeInAnimation->setEndValue(1.0);
    fadeInAnimation->setEasingCurve(QEasingCurve::Linear);

    int currentIndex = stack->currentIndex();
    int nextIndex = (currentIndex + 1) % stack->count(); // Loop back if index exceeds total number of widgets

    connect(fadeOutAnimation, &QPropertyAnimation::finished, this, [=]()
    {
        fadeInAnimation->start();
        stack->setCurrentIndex(nextIndex);
    });
}
