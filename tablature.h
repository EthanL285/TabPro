#ifndef TABLATURE_H
#define TABLATURE_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

class Tablature : public QWidget
{
    Q_OBJECT
public:
    explicit Tablature(QWidget *parent = nullptr);
    void addBarLine();
    void addRest();

public slots:
    void addFretNumber();
    void goLeft();
    void goRight();
    void play();

private slots:
    void selectColumn(bool checked);

private:
    QHBoxLayout *tabLayout;
    QPushButton *selectedColumn = nullptr;
    QVector<QPushButton*> columns;
};

#endif // TABLATURE_H
