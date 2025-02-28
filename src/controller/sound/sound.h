#ifndef SOUND_H
#define SOUND_H

#include <QObject>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>

class Sound : public QObject
{
    Q_OBJECT
public:
    explicit Sound(QObject *parent = nullptr);
    static void playNote(QString note);

signals:

};

#endif // SOUND_H
