#ifndef SPINBOX_H
#define SPINBOX_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class SpinBox : public QWidget
{
    Q_OBJECT
public:
    explicit SpinBox(int value, QPair<int, int> range, bool beatUnitBehaviour = false, QWidget *parent = nullptr);
    QPushButton *createArrowButton(int direction);
    void setValue(int value);
    void changeValue(int delta);

    bool isInRange(int value);
    bool isInvalidValue(int value);

private:
    QWidget *container;
    QLineEdit *field;
    QPushButton *upArrow;
    QPushButton *downArrow;
    bool beatUnitBehaviour;
    int min;
    int max;

private slots:
    void onTextChanged(QString text);

signals:
    void textChanged(int value);
};

#endif // SPINBOX_H
