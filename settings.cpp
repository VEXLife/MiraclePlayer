#include "settings.h"
#include "ui_settings.h"

Player::Channel Settings::source=Player::Netease;
Player::PlayEngine Settings::engine=Player::QtMediaPlayer;

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    settings=new QSettings("MiraclePlayerGroup","MiraclePlayer");
    if(settings->contains("Channel")){
        ui->channelSel->setCurrentIndex(settings->value("Channel").value<int>());
    }
    if(settings->contains("Engine")){
        ui->engineSel->setCurrentIndex(settings->value("Engine").value<int>());
    }
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_engineSel_currentIndexChanged(int index)
{
    engine=Player::PlayEngine(index);
    settings->setValue("Engine",QVariant::fromValue(index));
}


void Settings::on_channelSel_currentIndexChanged(int index)
{
    source=Player::Channel(index);
    settings->setValue("Channel",QVariant::fromValue(index));
}

