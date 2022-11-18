#ifndef SIDEBARITEM_H
#define SIDEBARITEM_H

#include <QWidget>
#include <QGraphicsOpacityEffect>

namespace Ui {
class SideBarItem;
}

class SideBarItem : public QWidget
{
    Q_OBJECT

public:
    explicit SideBarItem(QWidget *parent = nullptr);
    ~SideBarItem();
    void setSelected(bool selected);
    void setContent(QString icon,QString label);

private:
    Ui::SideBarItem *ui;
    QString icon; // 图标
    void setIcon(QString icon);
};

#endif // SIDEBARITEM_H
