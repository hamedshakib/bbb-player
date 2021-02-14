#ifndef HAMEDMEDIAPLAYER_H
#define HAMEDMEDIAPLAYER_H

#include <QObject>
#include <vlc/vlc.h>
#include <QWidget>
#include <QTimer>
#include <QUrl>

//class QTimer;
using HamedMedia = struct libvlc_media_t;
using HamedVlcInstance = struct libvlc_instance_t;
using HamedVlcMediaPlayer = struct libvlc_media_player_t;


class HamedMediaPlayer : public QObject
{
    Q_OBJECT
public:
    enum Status
    {
        NONE,
        OPENING,
        BUFFERING,
        PLAYING,
        PAUSED,
        STOPPED,
        END_OF_MEDIA,
        ERROR
    };

    HamedMediaPlayer(QObject *parent = nullptr);
    ~HamedMediaPlayer();
    void setVideoOutput(QWidget *videoWidget);
    void setMedia(const QUrl &url);
    qint64 mediaLenght();
    qint64 duration();
    float position();
    bool isPlaying();
    bool hasMedia();
    bool isMute();
    int volume();
    int status();
public slots:
    void setPosition(float pos);
    void setVolume(int volume);
    void setMute(bool mute);
    void play();
    void pause();
    void stop();
signals:
    void positionChanged(float);
    void mediaStatusChanged(int);
    void durationChanged(qint64);
private:
    QTimer *mTimer;
    HamedMedia *mMedia;
    QWidget *mVideoWidget;
    HamedVlcInstance *mInstance;
    HamedVlcMediaPlayer *mMediaPlayer;
};


#endif // HAMEDMEDIAPLAYER_H
