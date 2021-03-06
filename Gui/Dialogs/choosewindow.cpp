#include "choosewindow.h"
#include "ui_choosewindow.h"
#include <fstream>
#include <QStringList>

chooseWindow::chooseWindow(std::list<std::pair<QString,QString>>* filenames, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::chooseWindow),
    fnames{filenames}
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
    for(auto& n:*fnames)
        ui->listWidget->addItem(new QListWidgetItem(
                                    QIcon("resources/cont_red_circle.png"),n.first));
}



chooseWindow::~chooseWindow()
{
    delete ui;
}

void chooseWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    auto it=std::find_if(fnames->begin(),fnames->end(),[&](const std::pair<QString,QString> p){
        return p.first==item->text();
    });
    if(it!=fnames->end()){
      emit openfile(it->second.toStdString());
    }
    else
        emit openfileD();
}



void chooseWindow::on_pushButton_clicked()//open
{
    emit openfileD();
}

void chooseWindow::on_btn_new_clicked()
{
    emit newproject();
}
