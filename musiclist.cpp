#include "musiclist.h"
#include "ui_musiclist.h"

MusicList::MusicList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicList)
{
    /// 通用音乐列表
    ui->setupUi(this);
}

MusicList::~MusicList()
{
    delete ui;
}


void MusicList::addMusic(MusicItemData &data){
    MusicItem *item=new MusicItem(data);
    QListWidgetItem *ITEM = new QListWidgetItem(ui->content);
    ITEM->setSizeHint(QSize(400,48));
    ui->content->addItem(ITEM);
    data.faBtn=item->findChild<QPushButton*>("shou");
    ui->content->setItemWidget(ITEM, item);
}

void MusicList::addMusic(QList<MusicItemData> &data){
    for(int i=0;i<data.size();i++){
        data[i].index=i+1;
        addMusic(data[i]);
    }
}

void MusicList::removeMusic(const int &index) const{
    ui->content->takeItem(index);
}

void MusicList::clearMusic() const{
    ui->content->clear();
}

void MusicList::on_content_itemClicked(QListWidgetItem *item)
{
    ((MusicItem*)ui->content->itemWidget(item))->playSelf();
}
