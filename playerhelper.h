#ifndef PLAYMODE_H
#define PLAYMODE_H

namespace Player{
enum PlayMode{
    Order,Repeat,Shuffle
};
enum Channel{
    Netease,Kugou
};
enum PlayEngine{
    QtMediaPlayer,Win32ApiPlaySound
};
}

#endif // PLAYMODE_H
