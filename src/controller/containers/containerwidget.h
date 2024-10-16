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

    // Chord Window Functions
    void addChord(QVector<int> chord);
    void toggleChordMode();

signals:

private:
    MenuBar *menu;
    QWidget *rectangle;
    QHBoxLayout *guitarLayout;
    QWidget *playingTechniques;
    Tablature *tab;
    QVBoxLayout *tabLayout;
    Sound *sound;
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
