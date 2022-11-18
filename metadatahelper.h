#ifndef METADATAHELPER_H
#define METADATAHELPER_H

#include <musicdata.h>
#include <QString>
#include <QPixmap>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QObject>
#include <QEventLoop>
#include <QTimer>

namespace Player {
static void getMetaDataFromMusic(const QString &filePath,MusicItemData &music,QPixmap *cover){
    QMediaPlayer player;
    player.setSource(filePath);
    QEventLoop event;
    QObject::connect(&player,SIGNAL(metaDataChanged()),&event,SLOT(quit()));
    QTimer::singleShot(5000,&event,SLOT(quit()));
    event.exec();
    QMediaMetaData metaData=player.metaData();
    music.albumName=metaData.stringValue(QMediaMetaData::AlbumTitle);
    music.artist=metaData.stringValue(QMediaMetaData::Author);
    music.songName=metaData.stringValue(QMediaMetaData::Title);
    music.path=filePath;
    cover->loadFromData(metaData.value(QMediaMetaData::CoverArtImage).toByteArray());
    event.deleteLater();
}
}

#endif // METADATAHELPER_H
