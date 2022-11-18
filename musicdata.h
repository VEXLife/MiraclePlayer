#ifndef MUSICDATA_H
#define MUSICDATA_H

#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QPixmap>
#include <QBuffer>
#include <QPushButton>
#include <playerhelper.h>

class MusicItemData{
public:
    QString songName; // 歌曲名
    QString artist; // 艺术家
    QUrl path; // 路径
    QString albumName; // 专辑名字
    qint64 index=0; // 编号
    QString albumImgUrl; // 专辑图片路径
    QString lyricUrl; // 歌词网址
    Player::Channel source=Player::Netease; // 歌曲来源
    qint64 artistId; // 艺术家ID
    bool needRedirection; // 是否需要重定向请求

    bool operator==(const MusicItemData& d2) const{
        return path==d2.path;
    }

    QPushButton* faBtn=NULL; // 收藏中的按钮指针
};

class MusicBatchData{
public:
    QList<MusicItemData> musicList;

    // 利用友元函数重载流输入、流输出运算符
    friend QDataStream & operator << (QDataStream &arch, const MusicBatchData & object)
    {
        for(int i=0;i<object.musicList.count();i++){
            arch<<object.musicList.at(i).songName;
            arch<<object.musicList.at(i).artist;
            arch<<object.musicList.at(i).path;
            arch<<object.musicList.at(i).albumName;
            arch<<object.musicList.at(i).albumImgUrl;
            arch<<object.musicList.at(i).lyricUrl;
            arch<<object.musicList.at(i).artistId;
            arch<<object.musicList.at(i).source;
            arch<<object.musicList.at(i).needRedirection;
        }
        return arch;
    }

    friend QDataStream & operator >> (QDataStream &arch, MusicBatchData & object)
    {
        while(true){
            MusicItemData data;
            arch>>data.songName;
            arch>>data.artist;
            arch>>data.path;
            arch>>data.albumName;
            arch>>data.albumImgUrl;
            arch>>data.lyricUrl;
            arch>>data.artistId;
            arch>>data.source;
            arch>>data.needRedirection;
            if(arch.atEnd()) return arch;
            object.musicList.append(data);
        }
    }
};

Q_DECLARE_METATYPE(MusicBatchData) // 通知编译器记录元类型

#endif // MUSICDATA_H
