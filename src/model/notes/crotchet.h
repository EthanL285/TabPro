#ifndef CROTCHET_H
#define CROTCHET_H

#include "note.h"

#include <QWidget>

class Crotchet : public Note
{
    Q_OBJECT
public:
    explicit Crotchet(QVector<int> staffLines, QWidget *parent = nullptr);
    double getBeatValue() override;

protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif // CROTCHET_H
