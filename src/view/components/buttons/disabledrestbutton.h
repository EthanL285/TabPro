#ifndef DISABLEDRESTBUTTON_H
#define DISABLEDRESTBUTTON_H

#include <QWidget>
#include <QPushButton>

class DisabledRestButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DisabledRestButton(QWidget *parent = nullptr);

signals:
};

#endif // DISABLEDRESTBUTTON_H
