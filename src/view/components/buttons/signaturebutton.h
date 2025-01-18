#ifndef TIMESIGNATURE_H
#define TIMESIGNATURE_H

#include <QWidget>
#include <QPushButton>

class SignatureButton : public QPushButton
{
    Q_OBJECT
public:
    explicit SignatureButton(QString text, QSize size, int fontSize, QString toolTip, QWidget *parent = nullptr);

signals:
};

#endif // TIMESIGNATURE_H
