#include "sound.h"

#include <QTimer>

Sound::Sound(QObject *parent)
    : QObject{parent}
{
    player = new QMediaPlayer();
    output = new QAudioOutput();
    player->setAudioOutput(output);
    output->setVolume(1);
}

// Plays the given note
void Sound::playNote(QString note)
{
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
    player->setSource(QUrl(QString("qrc:/E/Sound/E/%1%2.wav").arg(map.value(string)).arg(fret)));
    player->play();
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
