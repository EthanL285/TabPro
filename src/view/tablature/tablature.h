#ifndef TABLATURE_H
#define TABLATURE_H

#include "sound.h"
#include "staff.h"
#include "tablaturebutton.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>

#define EMPTY_COLUMN "\u2015\n\u2015\n\u2015\n\u2015\n\u2015\n\u2015"

class Tablature : public QWidget
{
    Q_OBJECT
public:
    explicit Tablature(Sound *sound, Staff *staff, QWidget *parent = nullptr);
    void setPlayButton(QPushButton *button);
    QHBoxLayout *getLayout();
    QWidget *getLayoutItem(int index);
    QPushButton *createRest();
    QScrollArea *createScrollArea();
    QLabel *createNewTabLine();

    void updateTab();
    void resetTab();
    void addChord(QVector<int> chord);
    void addRest();
    void insertRest(int index);
    void adjustScrollBarPosition(QPushButton *button, QString alignment);
    void toggleChordMode();
    void removeColumn(int index, bool emitSignal);

    static constexpr int NUM_LINES = 6;
    static constexpr int LAYOUT_OFFSET = 2;
    static constexpr int BARLINE_HEIGHT = 79;

public slots:
    int getSelectedColumnIndex();
    void addFretNumber();
    void goLeft();
    void goRight();
    void playTab();
    void playColumn();
    void getColumnInfo(int index);
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
    void insertRestAfter();
    void insertBarLine();
    void undo();
    void remove();

private slots:
    void selectColumn(bool checked);

public slots:
    void onColumnRemoved(int index);
    void onRestInsertion(int index);

signals:
    void columnRemoved(int index);

private:
    QHBoxLayout *columnLayout;
    QVBoxLayout *rowLayout;
    QHBoxLayout *tabLayout;
    TablatureButton *selectedColumn = nullptr;
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
    int playIndex;
    int BPM = 60;
};

#endif // TABLATURE_H
