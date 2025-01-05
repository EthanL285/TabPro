#ifndef CONTAINERWIDGET_H
#define CONTAINERWIDGET_H

#include "menubar.h"
#include "tablature.h"
#include "sound.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class ContainerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ContainerWidget(MenuBar *menu, QWidget *parent = nullptr);
    QLabel *createStrings(QString note);
    void createFretBoard();
    void createFretButtons();
    void createPlayingTechniqueButtons();
    void createPlaybackButtons();
    QWidget *createTempoButton();
    QScrollArea *createScrollArea();

    // Testing Functions
    QWidget *getTabItem(int index);
    QWidget *getStaffItem(int index);
    QPushButton *getFretButton(int row, int col);
    QPushButton *getSelectedNoteButton(NoteType type);
    QPushButton *getUIButton(QString buttonName);
    QVector<RhythmSymbol*> getNotes();

    // Chord Window Functions
    void addChord(QVector<int> chord);
    void toggleChordMode();

signals:

private:
    MenuBar *menu;
    QWidget *rectangle;   
    QWidget *playingTechniques;
    Staff *staff;
    Tablature *tab;
    Sound *sound;

    QHBoxLayout *guitarLayout;
    QVBoxLayout *tabLayout;
    QGridLayout *fretLayout;
};

// Class for drawing rectangle
class Rectangle : public QWidget
{
    Q_OBJECT
public:
    explicit Rectangle(int width, int height, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int width;
    int height;
};

#endif // CONTAINERWIDGET_H
