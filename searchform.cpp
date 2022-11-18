#include "searchform.h"
#include "ui_searchform.h"

SearchForm::SearchForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchForm)
{
    ui->setupUi(this);
}

SearchForm::~SearchForm()
{
    delete ui;
}

void SearchForm::on_searchBtn_clicked()
{
    ui->musicList->clearMusic();
    QList<MusicItemData> result=searchMusic(ui->query->text());
    ui->musicList->addMusic(result);
}

QList<MusicItemData> SearchForm::searchMusic(const QString &query){
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply *reply;

    QList<MusicItemData> result; // 共享使用来减少资源消耗，用于存储结果

    QEventLoop eventLoop;
    switch(Settings::source){
    case Player::Netease:
        request.setUrl(QUrl(QString("http://cloud-music.pl-fe.cn/cloudsearch?keywords=").append(query)));
        reply=manager.get(request);
        QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();

        if(reply->error()==QNetworkReply::NoError){ // 假如请求成功
            QJsonDocument replyJson=QJsonDocument::fromJson(reply->readAll());
            QJsonArray songs=replyJson.object().value("result").toObject().value("songs").toArray();

            for(int i=0;i<songs.count();i++){
                MusicItemData data;
                QJsonObject song=songs.at(i).toObject();
                data.albumName=QString("《%1》").arg(song.value("al").toObject().value("name").toString());
                QJsonArray artists=song.value("ar").toArray();
                data.artist=QString();
                for(int j=0;j<artists.count();j++){
                    data.artist+=artists.at(j).toObject().value("name").toString();
                    if (j<artists.count()-1) data.artist.append('/');
                }
                data.artistId=artists.at(0).toObject().value("id").toInteger();
                data.lyricUrl=QString("http://cloud-music.pl-fe.cn/lyric?id=%1").arg(song.value("id").toInteger());

                data.path=QString("https://music.163.com/song/media/outer/url?id=%1.mp3").arg(song.value("id").toInteger());
                data.needRedirection=true;
                data.albumImgUrl=song.value("al").toObject().value("picUrl").toString();

                data.songName=song.value("name").toString();
                result.append(data);
            }
        }else{
            QMessageBox::critical(nativeParentWidget(),"错误","不能请求到在线音乐资源！");
        }
        break;
    case Player::Kugou:
        request.setUrl(QUrl(QString("http://mobilecdn.kugou.com/api/v3/search/song?format=json&page=1&pagesize=30&showtype=1&keyword=").append(query)));
        reply=manager.get(request);
        QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();

        if(reply->error()==QNetworkReply::NoError){ // 假如请求成功
            QJsonDocument replyJson=QJsonDocument::fromJson(reply->readAll());
            QJsonArray songs=replyJson.object().value("data").toObject().value("info").toArray();

            for(int i=0;i<songs.count();i++){
                MusicItemData data;
                QJsonObject song=songs.at(i).toObject();
                data.albumName=QString("《%1》").arg(song.value("album_name").toString());
                data.artist=song.value("singername").toString();
                data.path=QString("https://www.kugou.com/yy/index.php?r=play/getdata&hash=%1&album_id=%2").arg(song.value("hash").toString(), song.value("album_id").toString());
                data.needRedirection=true;
                data.songName=song.value("songname").toString();
                data.source=Player::Kugou;
                result.append(data);
            }
        }else{
            QMessageBox::critical(nativeParentWidget(),"错误","不能请求到在线音乐资源！");
        }
        break;
    }
    reply->deleteLater();
    manager.deleteLater();
    eventLoop.deleteLater();
    return result;
}
