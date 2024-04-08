#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIcon>
#include <QPixmap>
#include <QLabel>
#include <QResizeEvent>
#include <QImage>
#include <QPainter>
#include <QGraphicsColorizeEffect>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void redirectRegister();
    void redirectLogin();

private:
    Ui::MainWindow *ui;
    QPixmap backgroundImage;
    QLabel *backgroundLabel;
    QWidget *loginBox;
    QWidget *registerBox;
    QStackedWidget *stackedWidget;
};
#endif // MAINWINDOW_H
