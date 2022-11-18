#include "albumitem.h"
#include "ui_albumitem.h"


AlbumItem::AlbumItem(MusicItemData &musicitemdata, QWidget *parent) :
    QWidget(parent),musicitemdata(musicitemdata),
    ui(new Ui::AlbumItem)
{
    ui->setupUi(this);
}

AlbumItem::~AlbumItem()
{
    delete ui;
}

void AlbumItem::on_heart_clicked()
{
    loveMe();
}

void AlbumItem::loveMe(bool force){
    ((FavView*)nativeParentWidget()->findChild<QStackedWidget*>("content")->widget(1))->addFav(musicitemdata,force);
}

void AlbumItem::on_play_pause_clicked()
{
    nativeParentWidget()->findChild<PlayerControl*>("playerControl")->play_music(musicitemdata);
}
