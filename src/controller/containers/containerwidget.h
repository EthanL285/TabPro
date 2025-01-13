#ifndef CONTAINERWIDGET_H
#define CONTAINERWIDGET_H

#include "menubar.h"
#include "tablature.h"
#include "sound.h"
#include "mainwindow.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class ContainerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ContainerWidget(MainWindow *window, QWidget *parent = nullptr);
    QLabel *createStrings(QString note);
    void createFretBoard();
    void createFretButtons();
    void createPlayingTechniqueButtons();
    void createPlaybackButtons();
    QWidget *createTempoButton();
    QScrollArea *createScrollArea();

    void clearTab();

    // Testing Functions
    QWidget *getTabItem(int index);
    QWidget *getStaffItem(int index);
    QPushButton *getFretButton(int row, int col);
    QPushButton *getMenuButton(NoteType type);
    QPushButton *getMenuButton(QString buttonName);
    QPushButton *getUIButton(QString buttonName);
    const QVector<RhythmSymbol*> &getNotes();
    int getLayoutSize();

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
