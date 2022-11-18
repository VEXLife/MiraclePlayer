#ifndef ALBUMVIEW_H
#define ALBUMVIEW_H

#include <QWidget>
#include <QListWidgetItem>
#include <musicdata.h>
#include <playercontrol.h>
#include <musicitem.h>
#include <musiclist.h>
#include <mainwindow.h>
#include <albumitem.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QRandomGenerator>
#include <playerhelper.h>
#include <QFileDialog>
#include <metadatahelper.h>

namespace Ui {
class AlbumView;
}

class AlbumView : public QWidget
{
    Q_OBJECT

public:
    explicit AlbumView(QWidget *parent = nullptr);
    ~AlbumView();
    void addToPlaylist(MusicItemData&);
    void addToPlaylist(MusicBatchData&);
    void addLocalFiles(QStringList &targets);
    void updateAlbumInfo(MusicItemData&,QPixmap *cover);
    void playNextMusic(const Player::PlayMode &);
    void playPrevMusic(const Player::PlayMode &);

private slots:
    void on_playall_clicked();

    void on_loveall_clicked();

    void on_musicList_itemClicked(QListWidgetItem *item);

    void on_browseLocal_clicked();

private:
    Ui::AlbumView *ui;
};

#endif // ALBUMVIEW_H
