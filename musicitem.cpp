#include "mainwindow.h"
#include "musicitem.h"
#include "ui_musicitem.h"

MusicItem::MusicItem(MusicItemData &_data,QWidget *parent) :
    QWidget(parent),
    data(_data),
    ui(new Ui::MusicItem)
{
    /// 单个音乐项
    ui->setupUi(this);
    ui->gqm->setText(_data.songName);
    ui->gs->setText(_data.artist);
    ui->zj->setText(_data.albumName);
    ui->xh->setText(QString::number(_data.index,10));
}

MusicItem::~MusicItem()
{
    delete ui;
}

void MusicItem::on_bf_clicked()
{
    playSelf();
}

void MusicItem::playSelf(){
    nativeParentWidget()->findChild<PlayerControl*>("playerControl")->play_music(data);
}

void MusicItem::on_shou_clicked()
{
    loveMe();
}

void MusicItem::loveMe(bool force){
    ((FavView*)nativeParentWidget()->findChild<QStackedWidget*>("content")->widget(1))->addFav(data,force);
}
