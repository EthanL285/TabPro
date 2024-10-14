#ifndef MENUBAR_H
#define MENUBAR_H

#include <QWidget>
#include <QMenuBar>
#include <QPushButton>
#include <QHBoxLayout>

class MenuBar : public QWidget
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = nullptr);
    QPushButton *createButton(QString text, int fontSize);
    QHBoxLayout *createDivider();

private:
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QPushButton *selectedNote = nullptr;
    QPushButton *selectedSign = nullptr;

private slots:
    void clickNote();
    void clickSign();

signals:
};

#endif // MENUBAR_H
