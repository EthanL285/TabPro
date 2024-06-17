#ifndef GUITAR_H
#define GUITAR_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class Guitar : public QWidget
{
    Q_OBJECT
public:
    explicit Guitar(QWidget *parent = nullptr);
    QLabel *createStrings(QString note);
    void createFretBoard();
    void createFretButtons();

signals:

private:
    QWidget *rectangle;
    QHBoxLayout *guitarLayout;
};

// Class for drawing rectangle
class Rectangle : public QWidget
{
    Q_OBJECT
public:
    explicit Rectangle(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif // GUITAR_H
