#include "sidebaritem.h"
#include "ui_sidebaritem.h"

SideBarItem::SideBarItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideBarItem)
{
    ui->setupUi(this);
    setSelected(false); // 默认没有选中
}

SideBarItem::~SideBarItem()
{
    delete ui;
}

void SideBarItem::setContent(QString _icon, QString label){
    icon=_icon;
    setIcon(icon);
    ui->label->setText(label);
}

void SideBarItem::setIcon(QString icon){
    ui->icon->setStyleSheet(QString("image: url(:/mplayer/").append(icon).append(".svg);"));
}

void SideBarItem::setSelected(bool selected){
    if(selected){
        ui->decoration->setGraphicsEffect(NULL);
        ui->itemContainer->setStyleSheet("#itemContainer{\
                                         border-image: url(:/mplayer/mplayer-item.png);\
                                         border-radius:19px;\
                                         }\
#itemContainer *{\
background-color:rgba(0,0,0,0);\
}");
    setIcon(icon+"_w");
    ui->label->setStyleSheet("color:rgb(253,253,253);");
}else{
QGraphicsOpacityEffect *op=new QGraphicsOpacityEffect;
op->setOpacity(0);
ui->decoration->setGraphicsEffect(op);
ui->itemContainer->setStyleSheet("#itemContainer{\
                                 background-color: transparent;\
                                 border-radius:19px;\
                                 }\
#itemContainer *{\
background-color:rgba(0,0,0,0);\
}");
setIcon(icon);
ui->label->setStyleSheet("color:rgb(2,2,2);");
}
}
