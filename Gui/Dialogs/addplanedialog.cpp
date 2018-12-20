#include "addplanedialog.h"
#include "ui_addplanedialog.h"

addPlaneDialog::addPlaneDialog(const std::vector<material> *materials, balcalcItemModel* model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addPlaneDialog),bcmodel{model}
{
    ui->setupUi(this);
    mats=materials;
}

addPlaneDialog::~addPlaneDialog()
{
    delete ui;
}

void addPlaneDialog::setdata(bool tail, bool edit, int index){
    isTail=tail;
    curindex=index;

    ui->label_warning->hide();
    if(!bcmodel)return;

    if(mats){
        ui->mat_comboBox->clear();
        for(const material& m:*mats)
            ui->mat_comboBox->addItem(QString::fromStdString(m.name));
    }
    isXrfValid=isXrrValid=
            isXtfValid=isXtipValid=isXtrValid=
            isBrootValid=isBtipValid=isCrootValid=
            isCtipValid=isHvalid=isNvalid=false;
    if(tail){
        ui->label_X->hide();
        ui->le_X->setText(QString("5"));//условно 5 метров, обновится после установки ду
        ui->le_X->hide();
        isXvalid=true;
        if(edit){
            planeparams par{bcmodel->getheaddata().tailstabpar};
            ui->le_Btip->setText(QString("%1").arg(par.btip));
            ui->le_Croot->setText(QString("%1").arg(par.croot));
            ui->le_Ctip->setText(QString("%1").arg(par.ctip));
            ui->le_H->setText(QString("%1").arg(par.h));
            ui->le_N->setText(QString("%1").arg(par.n));
            ui->le_Xrf->setText(QString("%1").arg(par.xrf));
            ui->le_Xrr->setText(QString("%1").arg(par.xrr));
            ui->le_Xtf->setText(QString("%1").arg(par.xtf));
            ui->le_Xtip->setText(QString("%1").arg(par.xtip));
            ui->le_Xtr->setText(QString("%1").arg(par.xtr));
            ui->le_broot->setText(QString("%1").arg(par.broot));

            auto it=std::find(mats->begin(),mats->end(),par.mat);
            if(it!=mats->end()){
                ui->mat_comboBox->setCurrentIndex(it-mats->begin());
            }
        }
        else{
            ui->le_Btip->clear();
            ui->le_Croot->clear();
            ui->le_Ctip->clear();
            ui->le_H->clear();
            ui->le_N->clear();
            ui->le_Xrf->clear();
            ui->le_Xrr->clear();
            ui->le_Xtf->clear();
            ui->le_Xtip->clear();
            ui->le_Xtr->clear();
            ui->le_broot->clear();
        }
    }
    else{
        isXvalid=false;
        ui->label_X->show();
        ui->le_X->show();
        if(edit){
            planeparams par{bcmodel->getheaddata().planespar.at(size_t(index))};
            ui->le_X->setText(QString("%1").arg(par.XfromNose));
            ui->le_Btip->setText(QString("%1").arg(par.btip));
            ui->le_Croot->setText(QString("%1").arg(par.croot));
            ui->le_Ctip->setText(QString("%1").arg(par.ctip));
            ui->le_H->setText(QString("%1").arg(par.h));
            ui->le_N->setText(QString("%1").arg(par.n));
            ui->le_Xrf->setText(QString("%1").arg(par.xrf));
            ui->le_Xrr->setText(QString("%1").arg(par.xrr));
            ui->le_Xtf->setText(QString("%1").arg(par.xtf));
            ui->le_Xtip->setText(QString("%1").arg(par.xtip));
            ui->le_Xtr->setText(QString("%1").arg(par.xtr));
            ui->le_broot->setText(QString("%1").arg(par.broot));

            auto it=std::find(mats->begin(),mats->end(),par.mat);
            if(it!=mats->end()){
                ui->mat_comboBox->setCurrentIndex(it-mats->begin());
            }

        }
        else{
            ui->le_Btip->clear();
            ui->le_Croot->clear();
            ui->le_Ctip->clear();
            ui->le_H->clear();
            ui->le_N->clear();
            ui->le_Xrf->clear();
            ui->le_Xrr->clear();
            ui->le_Xtf->clear();
            ui->le_Xtip->clear();
            ui->le_Xtr->clear();
            ui->le_broot->clear();
            ui->le_X->clear();
        }
    }

}

void addPlaneDialog::on_le_broot_editingFinished()
{
    isBrootValid=true;
    double broot{0};
    if(ui->le_broot->text().isEmpty()|| (broot=ui->le_broot->text().toDouble())<=0){
        ui->label_warning->setText("параметр Broot введен неверно.");
        ui->label_warning->show();
        isBrootValid=false;
    }
}

void addPlaneDialog::on_le_Btip_editingFinished()
{
    isBtipValid=true;
    double btip{0};
    if(ui->le_Btip->text().isEmpty()|| (btip=ui->le_Btip->text().toDouble())<0){
        ui->label_warning->setText("параметр Btip введен неверно.");
        ui->label_warning->show();
        isBtipValid=false;
    }
}

void addPlaneDialog::on_le_Croot_editingFinished()
{
    isCrootValid=true;
    double croot{0};
    if(ui->le_Croot->text().isEmpty()|| (croot=ui->le_Btip->text().toDouble())<=0)
        ui->label_warning->setText("параметр Croot введен неверно.");
        ui->label_warning->show();
        isCrootValid=false;

}

void addPlaneDialog::on_le_Ctip_editingFinished()
{
    isCtipValid=true;
    double ctip{0};
    if(ui->le_Ctip->text().isEmpty()|| (ctip=ui->le_Ctip->text().toDouble())<0){
        ui->label_warning->setText("параметр Ctip введен неверно.");
        ui->label_warning->show();
        isCtipValid=false;
    }
}

void addPlaneDialog::on_le_Xtip_editingFinished()
{
    isXtipValid=true;
    double xtip{0};
    if(ui->le_Xtip->text().isEmpty()|| (xtip=ui->le_Xtip->text().toDouble())<=0){
        ui->label_warning->setText("параметр Xtip введен неверно.");
        ui->label_warning->show();
        isXtipValid=false;
    }
}

void addPlaneDialog::on_le_X_editingFinished()
{
    isXvalid=true;
    double x{0};
    if(ui->le_X->text().isEmpty()|| (x=ui->le_X->text().toDouble())<=0){
        ui->label_warning->setText("параметр X введен неверно.");
        ui->label_warning->show();
        isXvalid=false;
    }
}

void addPlaneDialog::on_le_N_editingFinished()
{

}

void addPlaneDialog::on_le_Xrf_editingFinished()
{

}

void addPlaneDialog::on_le_Xrr_editingFinished()
{

}

void addPlaneDialog::on_le_Xtf_editingFinished()
{

}

void addPlaneDialog::on_le_Xtr_editingFinished()
{

}

void addPlaneDialog::on_le_H_editingFinished()
{

}
