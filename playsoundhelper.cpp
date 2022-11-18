#include "playsoundhelper.h"

bool PlaySoundHelper::playing=false;
wchar_t PlaySoundHelper::url[500];

BOOL PlaySoundHelper::play(){
    playing=true;
    return PlaySoundW(url,NULL,SND_FILENAME|SND_APPLICATION|SND_ASYNC);
}

void PlaySoundHelper::pause(){
    playing=false;
    PlaySoundW(NULL,NULL,SND_ASYNC);
}

void PlaySoundHelper::setUrl(const QString &input){
    QString urlstr=input;
    urlstr.replace('/','\\').toWCharArray(url);
}
