#ifndef TOGGLESWITCH_H
#define TOGGLESWITCH_H

#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QPropertyAnimation>

class ToggleSwitch : public QWidget {
    Q_OBJECT

public:
    ToggleSwitch(QColor background, QWidget *parent = nullptr);
    bool isToggled();
    void toggle();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked();

private:
    QColor background;
    QWidget *handle;
    bool toggled = false;

public slots:
    void animateHandle();
};

#endif // TOGGLESWITCH_H
