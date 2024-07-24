#ifndef MENUBAR_H
#define MENUBAR_H

#include <QWidget>
#include <QMenuBar>

class MenuBar : public QWidget
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = nullptr);

private:
    QMenuBar *menuBar;
    QMenu *fileMenu;

signals:
};

#endif // MENUBAR_H
