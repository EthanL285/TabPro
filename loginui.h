#ifndef LOGINUI_H
#define LOGINUI_H

#include "mainwindow.h"
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>

class loginUI : public QWidget
{
    Q_OBJECT
public:
    explicit loginUI(MainWindow *parent = nullptr);

protected:
    MainWindow *mainWindow;

signals:
};

class RectangleWidget : public QWidget
{
public:
    RectangleWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    QSize sizeHint() const override;

private:
    const double aspectRatio = 2.0 / 3.0;
};

#endif // LOGINUI_H
