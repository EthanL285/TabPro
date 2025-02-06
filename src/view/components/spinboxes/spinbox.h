#ifndef SPINBOX_H
#define SPINBOX_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class SpinBox : public QWidget
{
    Q_OBJECT
public:
    explicit SpinBox(int value, QPair<int, int> range, QWidget *parent = nullptr);
    QPushButton *createArrowButton(int direction);
    void setValue(int value);
    void changeValue(int delta);

private:
    QWidget *container;
    QLineEdit *field;
    QPushButton *upArrow;
    QPushButton *downArrow;
    int min;
    int max;

private slots:
    void onTextChanged(QString text);

signals:
    void textChanged(int value);
};

#endif // SPINBOX_H
