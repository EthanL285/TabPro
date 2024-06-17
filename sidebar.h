#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPropertyAnimation>

class Sidebar : public QWidget
{
    Q_OBJECT
public:
    explicit Sidebar(QWidget *parent = nullptr);

public slots:
    void toggleSidebar();

private:
    QWidget *sidebar;
    QPropertyAnimation *sidebarAnimation = nullptr;

signals:
};

#endif // SIDEBAR_H
