#include "setnoseconedialog.h"
#include "ui_setnoseconedialog.h"

SetNoseConeDialog::SetNoseConeDialog(const std::vector<material>* materials, balcalcItemModel* model, QWidget *parent) :QDialog(parent),ui(new Ui::SetNoseConeDialog),bcmodel{model}{
    ui->setupUi(this);
    mats=materials;
}




SetNoseConeDialog::~SetNoseConeDialog()
{
    delete ui;
}

void SetNoseConeDialog::setdata(bool edit){
    ui->label_warning->hide();
    isDvalid=isLvalid=isdeltvalid=false;

    if(mats){
        ui->mat_comboBox->clear();
        std::cerr<<mats->size()<<std::endl;
        for(const material& m:*mats)
            ui->mat_comboBox->addItem(QString::fromStdString(m.name));

        if(!edit){
            ui->le_D->clear();
            ui->le_L->clear();
            ui->le_delt->clear();
        }
        else if(bcmodel){
            ui->le_D->setText(QString("%1").arg(bcmodel->getheaddata().nconepar.dend));

            ui->le_L->setText(QString("%1").arg(bcmodel->getheaddata().nconepar.len));
            ui->le_delt->setText(QString("%1").arg(bcmodel->getheaddata().nconepar.delt));
            auto it=std::find(mats->begin(),mats->end(),bcmodel->getheaddata().nconepar.mat);
            if(it!=mats->end()){
                ui->mat_comboBox->setCurrentIndex(it-mats->begin());
            }
        }
    }
}

void SetNoseConeDialog::checkD(){
    isDvalid=true;
    double D1{0};
    if(ui->le_D->text().isEmpty()|| (D1=ui->le_D->text().toDouble())<=0){
        ui->label_warning->setText("параметр D введен неверно.");
        ui->label_warning->show();
        isDvalid=false;
    }
}

void SetNoseConeDialog::checkL(){
    isLvalid=true;
    double L{0};
    if(ui->le_L->text().isEmpty()|| (L=ui->le_L->text().toDouble())<=0){
        ui->label_warning->setText("параметр L введен неверно.");
        ui->label_warning->show();
        isLvalid=false;
    }
}

void SetNoseConeDialog::checkdelt(){
    isdeltvalid=true;
    double delt{0};
    if(ui->le_delt->text().isEmpty()|| (delt=ui->le_delt->text().toDouble())<=0||
             ui->le_delt->text().toDouble()>0.5*ui->le_D->text().toDouble()){
        ui->label_warning->setText("параметр delt введен неверно.");
        ui->label_warning->show();
        isdeltvalid=false;
    }
}

bool SetNoseConeDialog::isDataValid(){
    checkD();
    checkL();
    checkdelt();
    return isDvalid && isLvalid && isdeltvalid;
}

void SetNoseConeDialog::on_SetNoseConeDialog_accepted(){
    if(isDataValid() && bcmodel && mats){
        auto mat_iterator=std::find_if(mats->begin(),mats->end(),
                             [&](const material& m){
            return m.name==ui->mat_comboBox->currentText().toStdString();});
        if(mat_iterator!=mats->end()){
                bcmodel->setNosecone(*mat_iterator,ui->le_D->text().toDouble(),
                                   ui->le_L->text().toDouble(),
                                   ui->le_delt->text().toDouble());
        }
    }
}

void SetNoseConeDialog::on_le_L_editingFinished()
{
   ui->label_warning->hide();
   checkL();
}

void SetNoseConeDialog::on_le_D_editingFinished()
{
    ui->label_warning->hide();
    checkD();
}

void SetNoseConeDialog::on_le_delt_editingFinished()
{
    ui->label_warning->hide();
    checkdelt();
}
