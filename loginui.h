#ifndef LOGINUI_H
#define LOGINUI_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>

class loginUI : public QWidget
{
    Q_OBJECT
public:
    explicit loginUI(QWidget *parent = nullptr);

signals:
};

class RectangleWidget : public QWidget
{
public:
    RectangleWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // LOGINUI_H
