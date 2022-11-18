#ifndef PLAYSOUNDHELPER_H
#define PLAYSOUNDHELPER_H
#ifdef _WINDOWS
#include <QObject>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

class PlaySoundHelper
{
public:
    static BOOL play(); // 播放
    static void pause(); // 暂停
    static bool playing; // 是否在播放
    static wchar_t url[]; // 播放路径
    static void setUrl(const QString &input);
};
#endif

#endif // PLAYSOUNDHELPER_H
