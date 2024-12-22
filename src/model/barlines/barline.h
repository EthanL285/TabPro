#ifndef BARLINE_H
#define BARLINE_H

#include <QWidget>

class BarLine : public QWidget
{
    Q_OBJECT
public:
    explicit BarLine(int height);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int barHeight;

signals:
};

#endif // BARLINE_H
