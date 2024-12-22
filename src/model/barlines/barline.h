#ifndef BARLINE_H
#define BARLINE_H

#include <QWidget>

class BarLine : public QWidget
{
    Q_OBJECT
public:
    explicit BarLine();

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
};

#endif // BARLINE_H
