#ifndef TABLATURE_H
#define TABLATURE_H

#include "sound.h"
#include "staff.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>

#define EMPTY_COLUMN "\u2015\n\u2015\n\u2015\n\u2015\n\u2015\n\u2015"
#define TAB_HEIGHT 92

class Tablature : public QWidget
{
    Q_OBJECT
public:
    explicit Tablature(Sound *sound, Staff *staff, QWidget *parent = nullptr);
    QPushButton *createRest();
    QScrollArea *createScrollArea();
    QLabel *createNewTabLine();
    void addRest();
    void adjustScrollBarPosition(QPushButton *button, QString alignment);
    void addChord(QVector<int> chord);
    void toggleChordMode();

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
    QVector<QPushButton*> tab;
    Sound *sound;
    QVector<int> *notes;
    QHash<int, QString> fretPositions;
    QTimer *tempo = nullptr;
    QPushButton *playButton = nullptr;
    QScrollArea *scrollArea = nullptr;
    Staff *staff = nullptr;
    bool playSwitch = true;
    bool chordMode = false;
    int index; // Index of column
    int BPM = 60;
};

#endif // TABLATURE_H
