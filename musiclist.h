#ifndef MUSICLIST_H
#define MUSICLIST_H

#include <QWidget>
#include <musicdata.h>
#include <musicitem.h>
#include <QListWidgetItem>

namespace Ui {
class MusicList;
}

class MusicList : public QWidget
{
    Q_OBJECT

public:
    explicit MusicList(QWidget *parent = nullptr);
    ~MusicList();
    void addMusic(MusicItemData &data);
    void addMusic(QList<MusicItemData> &data);
    void removeMusic(const int &index) const;
    void clearMusic() const;

private slots:
    void on_content_itemClicked(QListWidgetItem *item);

private:
    Ui::MusicList *ui;
};

#endif // MUSICLIST_H
