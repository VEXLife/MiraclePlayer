#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint); //设置无边框窗口
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    ui->sideBar->setStyleSheet("background-color: rgb(250, 251, 253);\nborder-top-left-radius:26px;");
    ui->mainContainer->setStyleSheet("background-color: rgb(255, 255, 255);\nborder-top-right-radius:26px;");
    ui->centralwidget->layout()->setContentsMargins(20,20,20,20);

    // 窗口阴影
    QGraphicsDropShadowEffect *effect=new QGraphicsDropShadowEffect();
    effect->setBlurRadius(20);
    effect->setColor(QColor::fromRgbF(0,0,0,0.3f));
    effect->setOffset(0,0);
    ui->centralwidget->setGraphicsEffect(effect);

    // 完成侧边栏
    QListWidgetItem *sideBarItem1 = new QListWidgetItem(ui->sideBarList);
    sideBarItem1->setSizeHint(QSize(100,52));
    ui->sideBarList->addItem(sideBarItem1);
    SideBarItem *albumViewItem=new SideBarItem(ui->sideBarList);
    albumViewItem->setContent("music","正在播放");
    ui->sideBarList->setItemWidget(sideBarItem1, albumViewItem);

    QListWidgetItem *sideBarItem2 = new QListWidgetItem(ui->sideBarList);
    sideBarItem2->setSizeHint(QSize(100,52));
    ui->sideBarList->addItem(sideBarItem2);
    SideBarItem *favItem=new SideBarItem(ui->sideBarList);
    favItem->setContent("heart","我的收藏");
    ui->sideBarList->setItemWidget(sideBarItem2, favItem);

    QListWidgetItem *sideBarItem3 = new QListWidgetItem(ui->sideBarList);
    sideBarItem3->setSizeHint(QSize(100,52));
    ui->sideBarList->addItem(sideBarItem3);
    SideBarItem *searchItem=new SideBarItem(ui->sideBarList);
    searchItem->setContent("globe","在线曲库");
    ui->sideBarList->setItemWidget(sideBarItem3, searchItem);

    QListWidgetItem *sideBarItem4 = new QListWidgetItem(ui->sideBarList);
    sideBarItem4->setSizeHint(QSize(100,52));
    ui->sideBarList->addItem(sideBarItem4);
    SideBarItem *settingsItem=new SideBarItem(ui->sideBarList);
    settingsItem->setContent("settings","播放设置");
    ui->sideBarList->setItemWidget(sideBarItem4, settingsItem);

    ui->sideBarList->setCurrentRow(0);

    //FIXME
    QStringList argList=QApplication::arguments();
    argList.removeFirst();
#ifdef Q_OS_WIN
    for (int i=0;i<argList.count();i++) {
        argList[i].replace("\\","/");
    }
#endif
    ui->album->addLocalFiles(argList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_close_clicked()
{
    this->close();
    exit(0);
}


void MainWindow::on_max_clicked()
{
    handleMaxMin();
    if(this->isMaximized()) this->showNormal(); else this->showMaximized();
}


void MainWindow::on_min_clicked()
{
    this->showMinimized();
}

void MainWindow::handleMaxMin(){
    if(!this->isMaximized()){
        ui->sideBar->setStyleSheet("background-color: rgb(250, 251, 253);");
        ui->mainContainer->setStyleSheet("background-color: rgb(255, 255, 255);");
        ui->playerControl->setRadius(0,*ui->playerControl);
        ui->centralwidget->layout()->setContentsMargins(0,0,0,0);
    }else{
        ui->sideBar->setStyleSheet("background-color: rgb(250, 251, 253);\nborder-top-left-radius:26px;");
        ui->mainContainer->setStyleSheet("background-color: rgb(255, 255, 255);\nborder-top-right-radius:26px;");
        ui->playerControl->setRadius(26,*ui->playerControl);
        ui->centralwidget->layout()->setContentsMargins(20,20,20,20);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    /// 用于处理鼠标移过时改变鼠标指针图标。
    /// 需要所有控件开启mouseTracking属性。
    /// 即便如此，QListWidget也无法实现。
    isMousePressed=true;
    mousePosition=event->globalPosition()-this->pos();
    bool isLeftResizing=event->position().x()<edgeInset,
            isRightResizing=geometry().width()-event->position().x()<edgeInset,
            isTopResizing=event->position().y()<edgeInset,
            isBottomResizing=geometry().height()-event->position().y()<edgeInset;
    if(isTopResizing&&isLeftResizing){
        resizing=Resizing::TOPLEFT;
    }else if(isTopResizing&&isRightResizing){
        resizing=Resizing::TOPRIGHT;
    }else if(isBottomResizing&&isLeftResizing){
        resizing=Resizing::BOTTOMLEFT;
    }else if(isBottomResizing&&isRightResizing){
        resizing=Resizing::BOTTOMRIGHT;
    }else if(isTopResizing){
        resizing=Resizing::TOP;
    }else if(isLeftResizing){
        resizing=Resizing::LEFT;
    }else if(isBottomResizing){
        resizing=Resizing::BOTTOM;
    }else if(isRightResizing){
        resizing=Resizing::RIGHT;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    /// 用于处理鼠标移过时改变鼠标指针图标。
    /// 需要所有控件开启mouseTracking属性。
    /// 即便如此，QListWidget也无法实现。
    if(isMousePressed&&event->buttons()==Qt::LeftButton){
        if(this->isMaximized()) {
            handleMaxMin();
            this->showNormal();
        }
        QPoint delta=(event->globalPosition()-mousePosition).toPoint();
        if(resizing==Resizing::NOTRESIZING){
            move(delta);
        }else{
            QRect geo=geometry();
            delta-=this->pos();
            switch(resizing){
            case TOPLEFT:
                geo.setTopLeft(geo.topLeft()+delta);
                break;
            case TOPRIGHT:
                geo.setTopRight(geo.topRight()+delta);
                break;
            case BOTTOMLEFT:
                geo.setBottomLeft(geo.bottomLeft()+delta);
                break;
            case BOTTOMRIGHT:
                geo.setBottomRight(geo.bottomRight()+delta);
                break;
            case TOP:
                geo.setTop(geo.top()+delta.y());
                break;
            case LEFT:
                geo.setLeft(geo.left()+delta.x());
                break;
            case BOTTOM:
                geo.setBottom(geo.bottom()+delta.y());
                break;
            case RIGHT:
                geo.setRight(geo.right()+delta.x());
            }
            this->setGeometry(geo);
        }
        mousePosition=event->globalPosition()-this->pos();
    }else{
        if(this->isMaximized()) {
            return;
        }
        bool isLeftResizing=event->position().x()<edgeInset,
                isRightResizing=geometry().width()-event->position().x()<edgeInset,
                isTopResizing=event->position().y()<edgeInset,
                isBottomResizing=geometry().height()-event->position().y()<edgeInset;
        if((isLeftResizing&&isTopResizing)
                ||(isRightResizing&&isBottomResizing)){
            this->setCursor(Qt::SizeFDiagCursor);
        }else if((isLeftResizing&&isBottomResizing)||(isRightResizing&&isTopResizing)){
            this->setCursor(Qt::SizeBDiagCursor);
        }else if(isLeftResizing||isRightResizing){
            this->setCursor(Qt::SizeHorCursor);
        }else if(isTopResizing||isBottomResizing){
            this->setCursor(Qt::SizeVerCursor);
        }else{
            this->unsetCursor();
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    /// 用于处理鼠标移过时改变鼠标指针图标。
    /// 需要所有控件开启mouseTracking属性。
    /// 即便如此，QListWidget也无法实现。
    isMousePressed=false;
    resizing=Resizing::NOTRESIZING;
}


void MainWindow::on_sideBarList_itemSelectionChanged()
{
    ((SideBarItem*)ui->sideBarList->itemWidget(ui->sideBarList->item(tabIndex)))->setSelected(false);
    ((SideBarItem*)ui->sideBarList->itemWidget(ui->sideBarList->currentItem()))->setSelected(true);
    tabIndex=ui->sideBarList->currentRow();
    ui->content->setCurrentIndex(tabIndex);
}

