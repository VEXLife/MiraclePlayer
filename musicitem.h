#ifndef MUSICITEM_H
#define MUSICITEM_H

#include <QWidget>
#include <playercontrol.h>
#include <musicdata.h>
#include <searchform.h>

namespace Ui {
class MusicItem;
}

class MusicItem : public QWidget
{
    Q_OBJECT

public:
    explicit MusicItem(MusicItemData &_data,QWidget *parent = nullptr);
    ~MusicItem();
    MusicItemData data; // 对应的数据
    void playSelf();
    void loveMe(bool force=false);
    void updateSelf() const;

private slots:
    void on_bf_clicked();

    void on_shou_clicked();

private:
    Ui::MusicItem *ui;
};

#endif // MUSICITEM_H
