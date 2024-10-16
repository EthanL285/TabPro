#include "sound.h"

#include <QTimer>

Sound::Sound(QObject *parent)
    : QObject{parent}
{}

// Plays the given note
void Sound::playNote(QString note)
{
    QMediaPlayer *player = new QMediaPlayer(this);
    QAudioOutput *output = new QAudioOutput(this);
    player->setAudioOutput(output);
    output->setVolume(1);

    // Retrieve string and fret number
    QStringList parts = note.split(" ");
    int string = parts[0].toInt();
    int fret = parts[1].toInt();

    QMap<int, QString> map =
    {
        {0, "E"},
        {1, "A"},
        {2, "D"},
        {3, "G"},
        {4, "B"},
        {5, "e"}
    };
    player->setSource(QUrl(QString("qrc:/E/E/%1%2.wav").arg(map.value(string)).arg(fret)));
    player->play();

    // Delete player and output once finished
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status)
    {
        if (status == QMediaPlayer::EndOfMedia)
        {
            player->deleteLater();
            output->deleteLater();
        }
    });
}



/*
connect(player, &QMediaPlayer::mediaStatusChanged, this, &Sound::handleStateChanged);

void Sound::handleStateChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia)
    {
        qDebug() << "Deleted";
        delete player;
    }
}
*/
