#ifndef TIMESIGNATURE_H
#define TIMESIGNATURE_H

#include <QWidget>
#include <QPushButton>

class SignatureButton : public QPushButton
{
    Q_OBJECT
public:
    explicit SignatureButton(QWidget *parent = nullptr);

signals:
};

#endif // TIMESIGNATURE_H
