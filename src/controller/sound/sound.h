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
    void playNote(QString note);
    void handleStateChanged(QMediaPlayer::MediaStatus status);

signals:

};

#endif // SOUND_H
