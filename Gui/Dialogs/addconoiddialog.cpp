#include "addconoiddialog.h"
#include "ui_addconoiddialog.h"

addConoidDialog::addConoidDialog(balcalcItemModel* model,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addConoidDialog),bcmodel{model}
{
    ui->setupUi(this);
}

addConoidDialog::~addConoidDialog()
{
    delete ui;
}

void addConoidDialog::on_buttonBox_accepted()
{  
    if(bcmodel)bcmodel->addConoid(matherial("this",1100,1200),0.3,0.5,0.8,0.05);
}



void addConoidDialog::on_le_D1_editingFinished()
{

    ui->label_warning->hide();
    double D1{0};
    if(ui->le_D1->text().isEmpty()|| (D1=ui->le_D1->text().toDouble())<=0){
        ui->label_warning->setText("параметр d1 введен неверно.");
        ui->label_warning->show();
    }
}

void addConoidDialog::on_le_D2_editingFinished()
{
    ui->label_warning->hide();
    double D2{0};
    if(ui->le_D2->text().isEmpty()|| (D2=ui->le_D2->text().toDouble())<=0){
        ui->label_warning->setText("параметр D2 введен неверно.");
        ui->label_warning->show();
    }
}
