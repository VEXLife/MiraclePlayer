#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QIcon>
#include <musicdata.h>
#include <albumview.h>
#include <playerhelper.h>
#include <QMediaMetaData>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <QSettings>
#include <playsoundhelper.h>

namespace Ui {
class PlayerControl;
}

class PlayerControl : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerControl(QWidget *parent = nullptr);
    ~PlayerControl();
    void setRadius(int radius, PlayerControl &control);
    QMediaPlayer *player=new QMediaPlayer(this); // 播放器
    QAudioOutput *output=new QAudioOutput(); // 音频输出设备
    void play_music(MusicItemData &music,QPixmap *cover=new QPixmap());
    Player::PlayMode playMode=Player::PlayMode::Order; // 播放顺序

private slots:
    void on_playBtn_clicked();
    void on_music_positionChanged(qint64);
    void on_music_stateChanged(QMediaPlayer::PlaybackState);
    void on_musicBar_sliderMoved(int position);

    void on_pushButton_2_clicked();

    void on_switchMode_clicked();

    void on_pushButton_3_clicked();

    void on_volSlider_valueChanged(int value);

    void on_vol_clicked();

private:
    QSettings *settings;
    QIcon pauseIcon;
    QIcon playIcon;
    QPixmap defaultAvatar; // 默认艺术家头像
    Ui::PlayerControl *ui;
    QStringList lyric; // 歌词
    void playWithEngine(QUrl &url,const qint64 &pos=-1);
    void pauseWithEngine();
};

#endif // PLAYERCONTROL_H
