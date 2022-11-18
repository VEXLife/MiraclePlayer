#include "albumview.h"
#include "ui_albumview.h"

AlbumView::AlbumView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlbumView)
{
    ui->setupUi(this);
    ui->albumDisplay->setPixmap(QPixmap(":mplayer/defaultalbum.png").scaled(ui->albumDisplay->geometry().width(),ui->albumDisplay->geometry().height(),Qt::KeepAspectRatioByExpanding));
}

AlbumView::~AlbumView()
{
    delete ui;
}

void AlbumView::addToPlaylist(MusicItemData &musicitemdata){
    // 防止重复添加
    for(int i=0;i<ui->musicList->count();i++){
        if(((AlbumItem*)ui->musicList->itemWidget(ui->musicList->item(i)))->musicitemdata.path==musicitemdata.path){
            return;
        }
    }

    QListWidgetItem *ITEM = new QListWidgetItem(ui->musicList);
    ITEM->setSizeHint(QSize(300,58));
    ui->musicList->addItem(ITEM);
    AlbumItem  *albumitem = new AlbumItem(musicitemdata, ui->musicList);
    albumitem->findChild<QLabel*>("song_name")->setText(musicitemdata.songName);
    ui->musicList->setItemWidget(ITEM,albumitem);

    if(((FavView*)((QStackedWidget*)parent())->widget(1))->favourites->contains(musicitemdata))
        albumitem->findChild<QPushButton*>("heart")->setIcon(QIcon(":mplayer/heart_fill_red")); // 更新爱心图标
}

void AlbumView::addToPlaylist(MusicBatchData &musicalbumdata){
    for (int i=0;i!=musicalbumdata.musicList.size();++i) {
        addToPlaylist(musicalbumdata.musicList.data()[i]);
    }
}

void AlbumView::updateAlbumInfo(MusicItemData &data,QPixmap *cover){
    if(cover->isNull()){
    // 请求封面图片
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(data.albumImgUrl);
    QNetworkReply *reply=manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    cover->loadFromData(reply->readAll());
    if(cover->isNull())  cover->load(":mplayer/defaultalbum");
    }
    ui->albumDisplay->setPixmap(cover->scaled(350,ui->albumDisplay->geometry().height(),Qt::KeepAspectRatio));
    ui->albumName->setText(QFontMetrics(ui->albumName->font()).elidedText(data.albumName,Qt::ElideRight,ui->albumName->width()));
    ui->artist->setText(QFontMetrics(ui->artist->font()).elidedText(data.artist,Qt::ElideRight,ui->artist->width()));

    // 更新正在播放的内容
    for(int i=ui->musicList->count()-1;i>=0;i--){
        if((((AlbumItem*)ui->musicList->itemWidget(ui->musicList->item(i)))->musicitemdata.path==data.path)){
            ui->musicList->setCurrentRow(i);
        }
    }
}

void AlbumView::on_playall_clicked()
{
    if(ui->musicList->count()>0){
        nativeParentWidget()->findChild<PlayerControl*>("playerControl")
                ->play_music(((AlbumItem*)ui->musicList->itemWidget(ui->musicList->item(0)))->musicitemdata);
    }
}



void AlbumView::on_loveall_clicked()
{
    if(ui->musicList->count()>0){
        for(int i=0;i!=ui->musicList->count();++i){
                ((AlbumItem*)ui->musicList->itemWidget(ui->musicList->item(i)))->loveMe(true);
        }
    }
}


void AlbumView::on_musicList_itemClicked(QListWidgetItem *item)
{
    nativeParentWidget()->findChild<PlayerControl*>("playerControl")
            ->play_music(((AlbumItem*)ui->musicList->itemWidget(item))->musicitemdata);
}

void AlbumView::playNextMusic(const Player::PlayMode &playMode){
    if(ui->musicList->count()<=0) return;
    MusicItemData target;
    switch(playMode){
    case Player::PlayMode::Order:
        target= ((AlbumItem*)ui->musicList->itemWidget(ui->musicList->item(ui->musicList->currentRow()+1<ui->musicList->count()?ui->musicList->currentRow()+1:0)))->musicitemdata;
        break;
    case Player::PlayMode::Shuffle:
        target= ((AlbumItem*)ui->musicList->itemWidget(ui->musicList->item(QRandomGenerator::global()->bounded(ui->musicList->count()))))->musicitemdata;
        break;
    case Player::PlayMode::Repeat:
        target= ((AlbumItem*)ui->musicList->itemWidget(ui->musicList->item(ui->musicList->currentRow())))->musicitemdata;
        break;
    }
    nativeParentWidget()->findChild<PlayerControl*>("playerControl")
            ->play_music(target);
}

void AlbumView::playPrevMusic(const Player::PlayMode &playMode){
    if(ui->musicList->count()<=0) return;
    MusicItemData target;
    switch(playMode){
    case Player::PlayMode::Order:
        target= ((AlbumItem*)ui->musicList->itemWidget(ui->musicList->item(ui->musicList->currentRow()-1>=0?ui->musicList->currentRow()-1:ui->musicList->count()-1)))->musicitemdata;
        break;
    case Player::PlayMode::Shuffle:
        target= ((AlbumItem*)ui->musicList->itemWidget(ui->musicList->item(QRandomGenerator::global()->bounded(ui->musicList->count()))))->musicitemdata;
        break;
    case Player::PlayMode::Repeat:
        target= ((AlbumItem*)ui->musicList->itemWidget(ui->musicList->item(ui->musicList->currentRow())))->musicitemdata;
        break;
    }
    nativeParentWidget()->findChild<PlayerControl*>("playerControl")
            ->play_music(target);
}

void AlbumView::on_browseLocal_clicked()
{
QStringList targets=QFileDialog::getOpenFileNames(nativeParentWidget(),"浏览本地音乐","","音乐文件 (*.wav *.mp3 *.wma *.mpg *.ogg *.aac *.flac);;所有文件 (*.*)");
addLocalFiles(targets);
}

void AlbumView::addLocalFiles(QStringList &targets){
    MusicItemData data;
    QPixmap cover;
    for(QString &target:targets){
        Player::getMetaDataFromMusic(target,data,&cover);
        addToPlaylist(data);
    }
    if(targets.count()>0) window()->findChild<PlayerControl*>("playerControl")
            ->play_music(data,&cover);
}
