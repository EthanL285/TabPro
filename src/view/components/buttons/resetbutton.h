#ifndef RESETBUTTON_H
#define RESETBUTTON_H

#include <QWidget>
#include <QPushButton>

class ResetButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ResetButton(QSize size, QString tooltip, QWidget *parent = nullptr);

signals:
};

#endif // RESETBUTTON_H
