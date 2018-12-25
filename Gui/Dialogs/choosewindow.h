#ifndef CHOOSEWINDOW_H
#define CHOOSEWINDOW_H

#include <QMainWindow>
#include "Gui/ItemModel/balcalcitemmodel.h"
#include "QListWidgetItem"
namespace Ui {
class chooseWindow;
}

class chooseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit chooseWindow(std::list<std::pair<QString,QString>>* filenames, QWidget *parent = nullptr);
    ~chooseWindow();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_pushButton_clicked();

    void on_btn_new_clicked();

private:
    Ui::chooseWindow *ui;
    QString fname;
    std::list<std::pair<QString,QString>>* fnames;
signals:
    void openfile(std::string fname);
    void openfileD();

};

#endif // CHOOSEWINDOW_H
