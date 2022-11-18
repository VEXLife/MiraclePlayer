#include "playercontrol.h"
#include "ui_playercontrol.h"

inline int getTimeFromStamp(const QString& input){
    // 内联的函数，用于找出歌词对应的时间并转换为毫秒数
    return input.mid(1,2).toInt()*60000+input.mid(4,2).toInt()*1000+input.mid(7,input.indexOf(']')-7+1).toInt();
}

PlayerControl::PlayerControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerControl)
{
    ui->setupUi(this);
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(on_music_positionChanged(qint64)));
    connect(player, SIGNAL(playbackStateChanged(QMediaPlayer::PlaybackState)), this, SLOT(on_music_stateChanged(QMediaPlayer::PlaybackState)));
    player->setAudioOutput(output);
    pauseIcon.addFile(QString::fromUtf8(":/mplayer/pause.svg"), QSize(), QIcon::Normal, QIcon::Off);
    playIcon.addFile(QString::fromUtf8(":/mplayer/play_w.svg"), QSize(), QIcon::Normal, QIcon::Off);
    defaultAvatar.load(":mplayer/cd");
    defaultAvatar=defaultAvatar.scaled(40,40);
    ui->volSlider->setVisible(false);

    settings=new QSettings("MiraclePlayerGroup","MiraclePlayer");
    if(settings->contains("Volume")){
        ui->volSlider->setValue(settings->value("Volume").value<int>());
    }
}

PlayerControl::~PlayerControl()
{
    delete player;
    delete output;
    delete ui;
}

void PlayerControl::setRadius(int radius, PlayerControl &control){
    /// 响应最大化时去掉圆角，是友元函数
    QString qss("#parentContainer{\n \
                border-image: url(:/mplayer/mplayer-bar.png);\n \
            border-bottom-left-radius:%1px;\n \
            border-bottom-right-radius:%1px;\n \
}");
qss=qss.arg(radius);
control.ui->parentContainer->setStyleSheet(qss);
}

void PlayerControl::on_playBtn_clicked()
{
    if((Settings::engine==Player::QtMediaPlayer
        &&player->playbackState()==QMediaPlayer::PlaybackState::PlayingState)
            ||(Settings::engine==Player::Win32ApiPlaySound
               &&PlaySoundHelper::playing)){
        pauseWithEngine();
    }else{
        QUrl empty;
        playWithEngine(empty);
    }
}

void PlayerControl::on_music_positionChanged(qint64 position){
    ui->musicBar->setMaximum(player->duration());

    ui->musicBar->setValue(position);
    int currentSecondSum=position/1000;
    int currentMinute=currentSecondSum/60;
    QString current("%1:%2");
    current=current.arg(currentMinute).arg(currentSecondSum%60,2,10,QLatin1Char('0'));
    ui->current->setText(current);

    int remainingSecondSum=(player->duration()-position)/1000;
    int remainingMinute=remainingSecondSum/60;
    QString remaining("%1:%2");
    remaining=remaining.arg(remainingMinute).arg(remainingSecondSum%60,2,10,QLatin1Char('0'));
    ui->remaining->setText(remaining);

    if (remainingSecondSum==0&&player->duration()>0)
        on_pushButton_2_clicked();

    // 显示歌词
    if(lyric.count()>0){
        int i;
        for (i=0;i<lyric.count()-1;i++) {
            if(getTimeFromStamp(lyric.at(i))<=position&&getTimeFromStamp(lyric.at(i+1))>position){
                break;
            }
        }
        ui->lyric->setText(lyric.at(i).mid(lyric.at(i).indexOf(']')+1));
        return;
    }
}


void PlayerControl::on_musicBar_sliderMoved(int position)
{
    player->setPosition(position);
}


void PlayerControl::on_music_stateChanged(QMediaPlayer::PlaybackState state){
    switch(state){
    case QMediaPlayer::PlayingState:
        ui->playBtn->setIcon(pauseIcon);
        ui->playBtn->setToolTip("暂停");
        break;
    default:
        ui->playBtn->setIcon(playIcon);
        ui->playBtn->setToolTip("播放");
        break;
    }
}


void PlayerControl::play_music(MusicItemData &music,QPixmap* cover){
    AlbumView *aview=window()->findChild<AlbumView*>("album");
    aview->addToPlaylist(music);
    QUrl redirectedUrl=music.path;
    lyric.clear();
    lyric<<"[00:00.000]暂无歌词。";
    ui->label->setPixmap(defaultAvatar);

    if(redirectedUrl.toString().startsWith("http")){
    // 请求资源
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply *reply=NULL;
    QEventLoop eventLoop;
    QJsonObject result;
    QPixmap cover;
    switch(music.source){
    case Player::Netease:
        if(music.needRedirection){
            request.setUrl(music.path);
            reply=manager.get(request);
            QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
            eventLoop.exec();
            redirectedUrl=QUrl(reply->rawHeader("location")); // 拿到重定向后的URL
            reply->deleteLater();
        }

        request.setUrl(QString("http://cloud-music.pl-fe.cn/artist/detail?id=%1").arg(music.artistId));
        reply=manager.get(request);
        QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
        reply->deleteLater();

        request.setUrl(QJsonDocument::fromJson(reply->readAll()).object().value("data").toObject().value("artist").toObject().value("cover").toString());
        reply=manager.get(request);
        QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
        cover.loadFromData(reply->readAll());
        ui->label->setPixmap(cover.scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatioByExpanding));
        reply->deleteLater();

        request.setUrl(music.lyricUrl);
        reply=manager.get(request);
        QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
        result=QJsonDocument::fromJson(reply->readAll()).object();
        if(!result.contains("uncollected")){
            lyric=result.value("lrc").toObject().value("lyric").toString().split('\n');
        }
        break;
    case Player::Kugou:
        if(music.needRedirection){
            request.setUrl(music.path);
            request.setRawHeader("cookie","kg_mid=c1a1900c8b0e291b8689836e1cbfa3a2"); // 设置Cookie
            reply=manager.get(request);
            QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
            eventLoop.exec();
            result=QJsonDocument::fromJson(reply->readAll()).object().value("data").toObject();
            redirectedUrl=QUrl(result.value("play_url").toString());
            music.albumImgUrl=result.value("img").toString();
            reply->deleteLater();

            request.setUrl(QUrl(result.value("authors").toArray().first().toObject().value("avatar").toString()));
            reply=manager.get(request);
            QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
            eventLoop.exec();
            cover.loadFromData(reply->readAll());
            ui->label->setPixmap(cover.scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatioByExpanding));

            if(result.contains("lyrics")){
                lyric=result.value("lyrics").toString().split('\n');
            }
        }
        break;
    }

    reply->deleteLater();
    eventLoop.deleteLater();
    manager.deleteLater();
    }

    aview->updateAlbumInfo(music,cover);
    ui->lyric->setText(music.songName+" - "+music.artist);
    playWithEngine(redirectedUrl,0);
}

void PlayerControl::on_pushButton_2_clicked()
{
    nativeParentWidget()->findChild<AlbumView*>("album")->playNextMusic(playMode);
}


void PlayerControl::on_switchMode_clicked()
{
    switch(playMode){
    case Player::Order:
        playMode=Player::Repeat;
        ui->switchMode->setIcon(QIcon(":mplayer/repeat"));
        ui->switchMode->setToolTip("单曲循环");
        break;
    case Player::Repeat:
        playMode=Player::Shuffle;
        ui->switchMode->setIcon(QIcon(":mplayer/shuffle"));
        ui->switchMode->setToolTip("随机播放");
        break;
    case Player::Shuffle:
        playMode=Player::Order;
        ui->switchMode->setIcon(QIcon(":mplayer/order"));
        ui->switchMode->setToolTip("顺序播放");
        break;
    }
}


void PlayerControl::on_pushButton_3_clicked()
{
    nativeParentWidget()->findChild<AlbumView*>("album")->playPrevMusic(playMode);
}
void PlayerControl::on_volSlider_valueChanged(int value)
{
    if(value==0){
        ui->vol->setIcon(QIcon(":mplayer/volume-x"));
    }else if(value<50){
        ui->vol->setIcon(QIcon(":mplayer/volume-1"));
    }else{
        ui->vol->setIcon(QIcon(":mplayer/volume-2"));
    }
    output->setVolume(1.0*value/ui->volSlider->maximum()); // 注意直接除会得到整数
    settings->setValue("Volume",QVariant::fromValue(value));
}


void PlayerControl::on_vol_clicked()
{
    ui->volSlider->setVisible(!ui->volSlider->isVisible());
}

void PlayerControl::playWithEngine(QUrl &url,const qint64 &pos){
    switch(Settings::engine){
    case Player::QtMediaPlayer:
        if(!url.isEmpty()) player->setSource(url);
        if(pos>0) player->setPosition(pos);
        output->setVolume(1.0*ui->volSlider->value()/ui->volSlider->maximum());
        player->play();
        if(player->mediaStatus()==QMediaPlayer::NoMedia){
            QMessageBox::critical(window(),"错误","音乐无法播放！");
        }
        break;
    case Player::Win32ApiPlaySound:
        if(!url.isEmpty()) PlaySoundHelper::setUrl(url.toString());
        if(PlaySoundHelper::play())
            on_music_stateChanged(QMediaPlayer::PlayingState);
        break;
    }
}


void PlayerControl::pauseWithEngine(){
    switch(Settings::engine){
    case Player::QtMediaPlayer:
        player->pause();
        break;
    case Player::Win32ApiPlaySound:
        PlaySoundHelper::pause();
        on_music_stateChanged(QMediaPlayer::PausedState);
        break;
    }
}
