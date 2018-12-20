#include "errordialog.h"
#include "ui_errordialog.h"
#include <iostream>

errorDialog::errorDialog(QString errtext,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::errorDialog)
{
    ui->setupUi(this);
    ui->label->setText(errtext);
    std::cout<<"activate errdial "<<errtext.toStdString()<<std::endl;
}

errorDialog::~errorDialog()
{
    delete ui;
}

void errorDialog::on_pushButton_clicked(){
    exit(1);
}
