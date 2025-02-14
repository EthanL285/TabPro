#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "transitions.h"
#include "authmanager.h"
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

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void redirectRegister();
    void redirectLogin();
    void redirectPassword();
    void redirectMainWidget();

private:
    QPixmap backgroundImage;
    QLabel *backgroundLabel;
    QWidget *loginBox;
    QWidget *registerBox = nullptr;
    QWidget *passwordBox = nullptr;
    QWidget *mainWidget = nullptr;
    QStackedWidget *stackedWidget = nullptr;
    Transitions *transition = nullptr;
    AuthManager *authManager = nullptr;
};
#endif // MAINWINDOW_H
