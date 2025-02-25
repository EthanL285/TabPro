#ifndef FILEMENU_H
#define FILEMENU_H

#include <QWidget>
#include <QMenu>

class FileMenu : public QMenu
{
    Q_OBJECT
public:
    explicit FileMenu(QWidget *parent = nullptr);

signals:
};

#endif // FILEMENU_H
