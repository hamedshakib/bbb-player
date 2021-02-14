#include "hamedmediaplayer.h"


HamedMediaPlayer::HamedMediaPlayer(QObject *parent)
    : QObject(parent)
{
    mMedia = nullptr;
    mTimer = new QTimer(this);
    mInstance = libvlc_new(0, nullptr);
    mMediaPlayer = libvlc_media_player_new(mInstance);

    connect(mTimer, &QTimer::timeout, [&]() {
        emit positionChanged(position());
        emit mediaStatusChanged(status());
        if (status() == END_OF_MEDIA) {
            mTimer->stop();
        }
    });
}

HamedMediaPlayer::~HamedMediaPlayer()
{
    libvlc_media_release(mMedia);
    libvlc_media_player_release(mMediaPlayer);
    libvlc_release(mInstance);
}

void HamedMediaPlayer::setVideoOutput(QWidget *videoWidget)
{
    mVideoWidget = videoWidget;
    libvlc_media_player_set_hwnd(mMediaPlayer, reinterpret_cast<HWND *>
                                 (mVideoWidget->winId()));
}

void HamedMediaPlayer::setMedia(const QUrl &url)
{
    libvlc_media_release(mMedia);
    mMedia = libvlc_media_new_location(mInstance,
                url.url().toStdString().c_str());
    libvlc_media_player_set_media(mMediaPlayer, mMedia);
    libvlc_media_parse(mMedia);
    emit durationChanged(duration());
    mTimer->start(1);
}

void HamedMediaPlayer::setPosition(float pos)
{
    libvlc_media_player_set_position(mMediaPlayer, pos);
}

float HamedMediaPlayer::position()
{
    return libvlc_media_player_get_position(mMediaPlayer);
}

void HamedMediaPlayer::setVolume(int volume)
{
    libvlc_audio_set_volume(mMediaPlayer, volume);
}

void HamedMediaPlayer::setMute(bool mute)
{
    libvlc_audio_set_mute(mMediaPlayer, mute);
}

int HamedMediaPlayer::volume()
{
    return libvlc_audio_get_volume(mMediaPlayer);
}

bool HamedMediaPlayer::isMute()
{
    return libvlc_audio_get_mute(mMediaPlayer);
}

void HamedMediaPlayer::play()
{
    libvlc_media_player_play(mMediaPlayer);
    emit mediaStatusChanged(status());
    mTimer->start(1);
}

bool HamedMediaPlayer::isPlaying()
{
    return libvlc_media_player_is_playing(mMediaPlayer);
}

void HamedMediaPlayer::pause()
{
    libvlc_media_player_pause(mMediaPlayer);
    emit mediaStatusChanged(status());
}

void HamedMediaPlayer::stop()
{
    mTimer->stop();
    libvlc_media_player_stop(mMediaPlayer);
    emit mediaStatusChanged(status());
}

int HamedMediaPlayer::status()
{
    return libvlc_media_player_get_state(mMediaPlayer);
}

bool HamedMediaPlayer::hasMedia()
{
    return libvlc_media_player_get_media(mMediaPlayer);
}

qint64 HamedMediaPlayer::duration()
{
    return libvlc_media_get_duration(mMedia);
}

qint64 HamedMediaPlayer::mediaLenght()
{
    return libvlc_media_player_get_length(mMediaPlayer);
}
