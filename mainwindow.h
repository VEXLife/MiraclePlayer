#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsDropShadowEffect>
#include <playercontrol.h>
#include <QMouseEvent>
#include <sidebaritem.h>
#include <albumview.h>
#include <musiclist.h>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_close_clicked();
    void on_max_clicked();
    void on_min_clicked();

    void on_sideBarList_itemSelectionChanged();

private:
    Ui::MainWindow *ui;
    void handleMaxMin();
    bool isMousePressed=false;
    enum Resizing{
        LEFT,TOP,BOTTOM,RIGHT,TOPLEFT,TOPRIGHT,BOTTOMLEFT,BOTTOMRIGHT,NOTRESIZING
    };
    Resizing resizing=Resizing::NOTRESIZING; // 指示鼠标停留区域便于设置指针
    QPointF mousePosition; // 鼠标位置
    const int edgeInset=30; // 鼠标图标变化的阈值
    int tabIndex=0; // 当前的选项卡序号

protected:
    virtual void mousePressEvent(QMouseEvent *event); // 重写鼠标相关方法
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
};
#endif // MAINWINDOW_H
