#ifndef GUITAR_H
#define GUITAR_H

#include "tablature.h"
#include "sound.h"

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class Guitar : public QWidget
{
    Q_OBJECT
public:
    explicit Guitar(QWidget *parent = nullptr);
    QLabel *createStrings(QString note);
    void createFretBoard();
    void createFretButtons();
    void createPlayingTechniqueButtons();
    void createPlaybackButtons();
    QWidget *createTempoButton();
    QScrollArea *createScrollArea();

signals:

private:
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

#endif // GUITAR_H
