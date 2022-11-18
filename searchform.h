#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QWidget>
#include <musicdata.h>
#include <QMessageBox>
#include <settings.h>

namespace Ui {
class SearchForm;
}

class SearchForm : public QWidget
{
    Q_OBJECT

public:
    explicit SearchForm(QWidget *parent = nullptr);
    ~SearchForm();

private slots:
    void on_searchBtn_clicked();

private:
    Ui::SearchForm *ui;
    QList<MusicItemData> searchMusic(const QString &query); // 搜索音乐
};

#endif // SEARCHFORM_H
