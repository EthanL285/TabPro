#ifndef QUAVER_H
#define QUAVER_H

#include "note.h"
#include "quaverfamily.h"

#include <QWidget>

class Quaver : public Note, public QuaverFamily
{
    Q_OBJECT
public:
    explicit Quaver(QVector<int> staffLines, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void drawFlag() override;
    void drawBeam() override;
};


#endif // QUAVER_H


