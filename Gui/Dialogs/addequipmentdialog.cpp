#include "addequipmentdialog.h"
#include "ui_addequipmentdialog.h"

addequipmentDialog::addequipmentDialog(balcalcItemModel* model, QWidget *parent):
    QDialog(parent),
    ui(new Ui::addequipmentDialog),
    bcmodel{model}
{
    ui->setupUi(this);
}


addequipmentDialog::~addequipmentDialog()
{
    delete ui;
}

bool addequipmentDialog::isDataValid()
{
 on_le_Xm_editingFinished();
 on_le_m_editingFinished();
 on_le_name_editingFinished();
 return isMassValid&&isXvalid&&isNameValid;
}

void addequipmentDialog::setdata(int index)
{
    ui->label_warning->hide();
    isMassValid=isXvalid=isNameValid=false;
    curindex=index;

    if(index==isNotIndex){
        ui->le_Xm->clear();
        ui->le_m->clear();
        ui->le_name->clear();
    }
    else{
        equipmentparameters par{bcmodel->getheaddata().equipspar.at(size_t(curindex))};
        ui->le_name->setText(QString::fromStdString(par.name));
        ui->le_Xm->setText(QString("%1").arg(par.X));
        ui->le_m->setText(QString("%1").arg(par.m));
    }
}

void addequipmentDialog::on_le_name_editingFinished()
{
    isNameValid=true;

    if(ui->le_name->text().isEmpty()){
        ui->label_warning->setText("Имя введено неверно.");
        ui->label_warning->show();
        isNameValid=false;
    }

}

void addequipmentDialog::on_le_Xm_editingFinished()
{
    isXvalid=true;
    double x{0};
    if(ui->le_Xm->text().isEmpty()|| (x=ui->le_Xm->text().toDouble())<=0){
        ui->label_warning->setText("параметр X введен неверно.");
        ui->label_warning->show();
        isXvalid=false;
    }
}

void addequipmentDialog::on_le_m_editingFinished()
{
    isMassValid=true;
    double mass{0};
    if(ui->le_m->text().isEmpty()|| (mass=ui->le_m->text().toDouble())<=0){
        ui->label_warning->setText("масса введена неверно.");
        ui->label_warning->show();
        isMassValid=false;
    }

}

void addequipmentDialog::on_buttonBox_accepted(){
    if(isDataValid() && bcmodel){
        if(curindex!=isNotIndex)
            bcmodel->ejectEquipment(size_t(curindex));

        bcmodel->addEquipment(ui->le_name->text().toStdString(),
                              ui->le_Xm->text().toDouble(),
                              ui->le_m->text().toDouble());
        accept();
    }
}
