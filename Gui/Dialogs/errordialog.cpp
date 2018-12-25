#include "errordialog.h"
#include "ui_errordialog.h"
#include <iostream>

errorDialog::errorDialog(QString errtext, bool critical,QWidget *parent) :
    QDialog(parent),crit{critical},
    ui(new Ui::errorDialog)
{
    ui->setupUi(this);
    ui->label->setText(errtext);
}

errorDialog::~errorDialog()
{
    delete ui;
}

void errorDialog::setdata(QString text, bool critical){
    ui->label->setText(text);
    crit=critical;
}

void errorDialog::on_pushButton_clicked(){
    if(crit)exit(1);
    hide();
}
