#ifndef TABLATURE_H
#define TABLATURE_H

#include "sound.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>

class Tablature : public QWidget
{
    Q_OBJECT
public:
    explicit Tablature(Sound *sound, QWidget *parent = nullptr);
    QPushButton *createRest();
    QScrollArea *createScrollArea();
    QLabel *createNewTabLine();
    void addRest();
    void resizeTab(int width);

public slots:
    int getSelectedColumnIndex();
    void addFretNumber();
    void goLeft();
    void goRight();
    void playTab(QPushButton *play);
    void playColumn();
    void getColumnInfo();
    void stopTempoTimer();

    // Techniques
    void changeTempoButton(QLineEdit *field, QPushButton *increase, QPushButton *decrease);
    void changeTempoEdit(QLineEdit *field, QPushButton *increase, QPushButton *decrease);
    void insertSlideUp();
    void insertSlideDown();
    void insertHammerOn();
    void insertPullOff();
    void insertBend();
    void insertRelease();
    void insertVibrato();
    void insertMutedHit();
    void insertRest();
    void insertBarLine();
    void undo();
    void remove();

private slots:
    void selectColumn(bool checked);

private:
    QHBoxLayout *columnLayout;
    QVBoxLayout *rowLayout;
    QHBoxLayout *tabLayout;
    QPushButton *selectedColumn = nullptr;
    QVector<QVector<QPushButton*>> tab;
    QVector<QPushButton*> columns;
    Sound *sound;
    QVector<int> *notes;
    QHash<int, QString> *fretPositions = nullptr;
    QTimer *tempo = nullptr;
    QPushButton *playButton = nullptr;
    bool playSwitch = true;
    int index; // Index of column
    int BPM = 60;
};

#endif // TABLATURE_H
