#ifndef FAVVIEW_H
#define FAVVIEW_H

#include <QWidget>
#include <mainwindow.h>
#include <musicdata.h>
#include <QSettings>

namespace Ui {
class FavView;
}

class FavView : public QWidget
{
    Q_OBJECT

public:
    explicit FavView(QWidget *parent = nullptr);
    ~FavView();
    void addFav(MusicItemData &data,const bool force=false) const;
    QList<MusicItemData> *favourites; // 已收藏的数据

private slots:
    void on_playall_clicked();

    void on_removeAll_clicked();

private:
    Ui::FavView *ui;
    QSettings *settings;
};

#endif // FAVVIEW_H
