#ifndef QUAVER_H
#define QUAVER_H

#include "note.h"

#include <QWidget>

class Quaver : public Note
{
    Q_OBJECT
public:
    explicit Quaver(QVector<int> staffLines, QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event) override;
    void drawFlag();
};


#endif // QUAVER_H


