#ifndef TABLATURE_H
#define TABLATURE_H

#include "sound.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

class Tablature : public QWidget
{
    Q_OBJECT
public:
    explicit Tablature(Sound *sound, QWidget *parent = nullptr);
    void addBarLine();
    void addRest();

public slots:
    void addFretNumber();
    void goLeft();
    void goRight();
    void playTab(double ms);
    void playColumn();
    void getColumnInfo();
    int getSelectedColumnIndex();

private slots:
    void selectColumn(bool checked);

private:
    QHBoxLayout *tabLayout;
    QPushButton *selectedColumn = nullptr;
    QVector<QPushButton*> columns;
    Sound *sound;
    QVector<int> *notes;
    QHash<int, QString> *fretPositions = nullptr;
    QTimer *tempo;
    int index; // Index of column
};

#endif // TABLATURE_H
