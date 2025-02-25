#ifndef TABLATURE_H
#define TABLATURE_H

#include "sound.h"
#include "staff.h"
#include "tabcolumn.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QTimer>
#include <QButtonGroup>

class Tablature : public QWidget
{
    Q_OBJECT
public:
    explicit Tablature(MenuBar *menu, Sound *sound, Staff *staff, QWidget *parent = nullptr);

    // Tab functions
    void updateTab();
    void resetTab();

    // Chord Functions
    void addChord(QVector<int> chord);
    void toggleChordMode();

    // Column Functions
    TabColumn *createColumn();
    void addLeadColumn();
    void addColumn(int index, const QVector<int> &frets, RhythmSymbol *symbol);
    void replaceColumn(int index, const QVector<int> &frets, RhythmSymbol *symbol);
    void removeColumn(int index);
    void selectColumn(int index);
    void selectColumn(TabColumn *column);

    // Testing functions
    QHBoxLayout *getLayout();
    QWidget *getLayoutItem(int index);

    static const inline QVector<int> EMPTY_COLUMN = {-1, -1, -1, -1, -1, -1};
    static constexpr int NUM_STRINGS = 6;
    static constexpr int LAYOUT_OFFSET = 2;
    static constexpr int BARLINE_HEIGHT = 79;
    static constexpr int DEFAULT_COLUMN_WIDTH = 35;
    static constexpr int EMPTY_FRET = -1;

public slots:
    int getSelectedColumnIndex();
    void addFretNumber(int fret, int string);
    void goLeft();
    void goRight();
    void playTab();
    void playColumn(int index);
    void pauseTab();

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
    void onColumnClick(bool checked);

signals:
    void columnRemoved(int index);
    void tabPaused();

private:
    MenuBar *menu;
    QHBoxLayout *columnLayout;
    QVBoxLayout *rowLayout;
    QHBoxLayout *tabLayout;
    TabColumn *selectedColumn = nullptr;
    QVector<TabColumn*> tab;
    QButtonGroup *buttons;
    Sound *sound;
    QVector<int> *notes;
    QTimer *tempo;
    QScrollArea *scrollArea = nullptr;
    Staff *staff = nullptr;
    bool isChordMode = false;
    int playIndex;
    int BPM = 60;

    void addColumnToLayout(int index, TabColumn *column);
    void adjustScrollBarPosition(TabColumn *column, QString alignment);

};

#endif // TABLATURE_H
