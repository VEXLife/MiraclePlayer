#ifndef ALBUMITEM_H
#define ALBUMITEM_H

#include <QWidget>
#include <musicdata.h>
#include <mainwindow.h>
#include <favview.h>
#include <QStackedWidget>
#include <searchform.h>

namespace Ui {
class AlbumItem;
}

class AlbumItem : public QWidget
{
    Q_OBJECT

public:
    explicit AlbumItem(MusicItemData &musicitemdata, QWidget *parent = nullptr);
    ~AlbumItem();
    MusicItemData musicitemdata; // 对应的音乐数据
    void loveMe(bool force=false);


private slots:
    void on_heart_clicked();

    void on_play_pause_clicked();

private:
    Ui::AlbumItem *ui;
};

#endif // ALBUMITEM_H
