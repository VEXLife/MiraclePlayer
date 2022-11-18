#include "favview.h"
#include "ui_favview.h"

FavView::FavView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FavView)
{
    ui->setupUi(this);

    // 存储收藏内容
    qRegisterMetaType<MusicBatchData>("MusicBatchData"); // 通知编译器记录元类型的流输入、流输出重载
    favourites=new QList<MusicItemData>();
    settings=new QSettings("MiraclePlayerGroup","MiraclePlayer");
    if(settings->contains("Favourites")){
        *favourites=settings->value("Favourites").value<MusicBatchData>().musicList;
    }
    ui->musicList->addMusic(*favourites);
    for(int i=0;i<favourites->count();i++)
        favourites->at(i).faBtn->setIcon(QIcon(":mplayer/heart_fill_red"));
}

FavView::~FavView()
{
    delete favourites;
    delete settings;
    delete ui;
}

void FavView::addFav(MusicItemData &data,const bool force) const{
    int pos=favourites->indexOf(data);

    // 更新正在播放列表中的小爱心图标
    QListWidget *mList=((AlbumView*)nativeParentWidget()->findChild<QStackedWidget*>("content")->widget(0))->findChild<QListWidget*>("musicList");
    QPushButton *alBtn=NULL;
    for(int i=0;i<mList->count();i++){
        if(((AlbumItem*)mList->itemWidget(mList->item(i)))->musicitemdata==data){
            alBtn=((AlbumItem*)mList->itemWidget(mList->item(i)))->findChild<QPushButton*>("heart");
        }
    }

    /**
     * 注意移除一项后，内存也会被擦除（即delete）。
     * 这将出现空指针和野指针。微软自带的CDB调不出这个问题，所以很难发现。
     * 为了解决这个问题，在访问时总是判断。
     */
    if(pos==-1){
        favourites->append(data);
        ui->musicList->addMusic(data);
        if(alBtn) alBtn->setIcon(QIcon(":mplayer/heart_fill_red"));
        data.faBtn->setIcon(QIcon(":mplayer/heart_fill_red"));
    }else if(!force){
        if(alBtn) {
            alBtn->setIcon(QIcon(":mplayer/heart"));
        }
        favourites->removeAt(pos);
        ui->musicList->removeMusic(pos);
    }

    // 保存数据
    MusicBatchData _data;
    _data.musicList=*favourites;
    settings->setValue("Favourites",QVariant::fromValue(_data));
}

void FavView::on_playall_clicked()
{
    if(favourites->count()>0){
        MusicBatchData batch;
        batch.musicList=*favourites;
        ((AlbumView*)nativeParentWidget()->findChild<QStackedWidget*>("content")->widget(0))->addToPlaylist(batch);
        nativeParentWidget()->findChild<PlayerControl*>("playerControl")
                ->play_music(favourites->data()[0]);
    }
}


void FavView::on_removeAll_clicked()
{
    int length=favourites->count();
    for (int i=0;i<length;i++)
        addFav((*favourites)[0]);
}

