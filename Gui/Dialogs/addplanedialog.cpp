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
    isEdit=edit;
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
            isCtipValid=isHvalid=false;
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

bool addPlaneDialog::isDataValid(){
    on_le_Btip_editingFinished();
    on_le_Croot_editingFinished();
    on_le_Ctip_editingFinished();
    on_le_H_editingFinished();
    on_le_X_editingFinished();
    on_le_Xrf_editingFinished();
    on_le_Xrr_editingFinished();
    on_le_Xtf_editingFinished();
    on_le_Xtip_editingFinished();
    on_le_Xtr_editingFinished();
    on_le_broot_editingFinished();
    return (isTail||isXvalid)&&
            isBrootValid&&
            isBtipValid&&
            isCrootValid&&
            isCtipValid&&
            isXtipValid&&
            isXrfValid&&
            isXrrValid&&
            isXtfValid&&
            isXtrValid&&
            isHvalid;
}

void addPlaneDialog::on_le_broot_editingFinished()
{
    ui->label_warning->hide();
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
    ui->label_warning->hide();
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
    ui->label_warning->hide();
    isCrootValid=true;
    double croot{0};
    if(ui->le_Croot->text().isEmpty()|| (croot=ui->le_Croot->text().toDouble())<=0){
        ui->label_warning->setText("параметр Croot введен неверно.");
        ui->label_warning->show();
        isCrootValid=false;
    }
}

void addPlaneDialog::on_le_Ctip_editingFinished()
{
    ui->label_warning->hide();
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
    ui->label_warning->hide();
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
    ui->label_warning->hide();
    isXvalid=true;
    double x{0};
    if(ui->le_X->text().isEmpty()|| (x=ui->le_X->text().toDouble())<=0){
        ui->label_warning->setText("параметр X введен неверно.");
        ui->label_warning->show();
        isXvalid=false;
    }
}


void addPlaneDialog::on_le_Xrf_editingFinished()
{
    ui->label_warning->hide();
    isXrfValid=true;
    double xrf{0};
    if(ui->le_Xrf->text().isEmpty()|| (xrf=ui->le_Xrf->text().toDouble())<0){
        ui->label_warning->setText("параметр Xrf введен неверно.");
        ui->label_warning->show();
        isXrfValid=false;
    }
}

void addPlaneDialog::on_le_Xrr_editingFinished()
{
    ui->label_warning->hide();
    isXrrValid=true;
    int xrr{0};
    if(ui->le_Xrr->text().isEmpty()|| (xrr=ui->le_Xrr->text().toDouble()<=0)){
        ui->label_warning->setText("параметр Xrr введен неверно.");
        ui->label_warning->show();
        isXrrValid=false;
    }
}

void addPlaneDialog::on_le_Xtf_editingFinished()
{
    ui->label_warning->hide();
    isXtfValid=true;
    double xtf{0};
    if(ui->le_Xtf->text().isEmpty()|| (xtf=ui->le_Xtf->text().toDouble())<0){
        ui->label_warning->setText("параметр Xtf введен неверно.");
        ui->label_warning->show();
        isXtfValid=false;
    }
}

void addPlaneDialog::on_le_Xtr_editingFinished()
{
    ui->label_warning->hide();
    isXtrValid=true;
    double xtr{0};
    if(ui->le_Xtr->text().isEmpty()|| (xtr=ui->le_Xtr->text().toDouble())<0){
        ui->label_warning->setText("параметр Xtr введен неверно.");
        ui->label_warning->show();
        isXtrValid=false;
    }
}

void addPlaneDialog::on_le_H_editingFinished()
{
    ui->label_warning->hide();
    isHvalid=true;
    double h{0};
    if(ui->le_H->text().isEmpty()|| (h=ui->le_H->text().toDouble())<=0){
        ui->label_warning->setText("параметр H введен неверно.");
        ui->label_warning->show();
        isHvalid=false;
    }
}

void addPlaneDialog::on_buttonBox_accepted()
{
    if(isDataValid() && bcmodel && mats){
        auto mat_iterator=std::find_if(mats->begin(),mats->end(),
                                       [&](const material& m){
            return m.name==ui->mat_comboBox->currentText().toStdString();});

        if(mat_iterator!=mats->end()){
            if(isTail){
                bcmodel->setTailStab(*mat_iterator,ui->le_broot->text().toDouble(),
                                     ui->le_Btip->text().toDouble(),
                                     ui->le_Croot->text().toDouble(),
                                     ui->le_Ctip->text().toDouble(),
                                     ui->le_Xtip->text().toDouble(),
                                     ui->le_Xrf->text().toDouble(),
                                     ui->le_Xrr->text().toDouble(),
                                     ui->le_Xtf->text().toDouble(),
                                     ui->le_Xtr->text().toDouble(),
                                     ui->le_H->text().toDouble()
                                     );
            }
            else{//не хвост
                if(isEdit)
                    bcmodel->ejectPlane(size_t(curindex));
                bcmodel->addplane(*mat_iterator,
                                  ui->le_X->text().toDouble(),
                                  ui->le_broot->text().toDouble(),
                                  ui->le_Btip->text().toDouble(),
                                  ui->le_Croot->text().toDouble(),
                                  ui->le_Ctip->text().toDouble(),
                                  ui->le_Xtip->text().toDouble(),
                                  ui->le_Xrf->text().toDouble(),
                                  ui->le_Xrr->text().toDouble(),
                                  ui->le_Xtf->text().toDouble(),
                                  ui->le_Xtr->text().toDouble(),
                                  ui->le_H->text().toDouble());//добавление
            }
            accept();
        }

    }
}

