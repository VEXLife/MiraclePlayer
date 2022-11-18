#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSettings>
#include <playerhelper.h>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    static Player::Channel source; // 音乐来源
    static Player::PlayEngine engine; // 播放引擎

private slots:
    void on_engineSel_currentIndexChanged(int index);

    void on_channelSel_currentIndexChanged(int index);

private:
    Ui::Settings *ui;
    QSettings *settings; // 设置信息指针
};

#endif // SETTINGS_H
