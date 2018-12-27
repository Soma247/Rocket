#include "setflytask.h"
#include "ui_setflytask.h"


SetFlyTask::SetFlyTask(const std::vector<material> *materials, const std::vector<fuel> *fuels, balcalcItemModel *model, QWidget *parent):
    QDialog(parent),ui(new Ui::SetFlyTask),
    mats{materials},
    fls{fuels},
    bcmodel{model}
{
    ui->setupUi(this);
}


SetFlyTask::~SetFlyTask()
{
    delete ui;
}

void SetFlyTask::setdata(bool edit)
{
    isHmaxValid=isHminValid=
            isXmaxValid=isMilestoneValid=
            isPaValid=isPkValid=
            isTethValid=isVtargetValid=false;
    ui->label_warning->hide();

    if(bcmodel&&mats&&fls){
        ui->cb_br->clear();
        ui->cb_cyl->clear();
        ui->cb_fuel->clear();
        ui->cb_nozzle->clear();
        ui->cb_tzp->clear();
        for(const material& m:*mats){
            ui->cb_br->addItem(QString::fromStdString(m.name));
            ui->cb_cyl->addItem(QString::fromStdString(m.name));
            ui->cb_nozzle->addItem(QString::fromStdString(m.name));
            ui->cb_tzp->addItem(QString::fromStdString(m.name));
        }
        for(const fuel& f:*fls)
            ui->cb_fuel->addItem(QString::fromStdString(f.name));

        InputData indat=bcmodel->getInputData();
        if(edit&&indat.iscorrect()){
            ui->le_Hmax->setText(QString("%1").arg(indat.Hmax));
            ui->le_Hmin->setText(QString("%1").arg(indat.Hmin));
            ui->le_Xmax->setText(QString("%1").arg(indat.Xmax));
            ui->le_Vtar->setText(QString("%1").arg(indat.Vtar));
            ui->le_mstone->setText(QString("%1").arg(indat.mstone));
            ui->le_Pk->setText(QString("%1").arg(indat.enPk));
            ui->le_Pa->setText(QString("%1").arg(indat.enPa));
            auto mat_cyl=std::find(mats->begin(),mats->end(),indat.enmatShell);
            auto mat_br=std::find(mats->begin(),mats->end(),indat.enmatbr);
            auto mat_tzp=std::find(mats->begin(),mats->end(),indat.enmatTz);
            auto mat_nozzle=std::find(mats->begin(),mats->end(),indat.enmatnozzle);
            auto fl_it=std::find(fls->begin(),fls->end(),indat.enfl);
            if(mat_cyl!=mats->end())
                ui->cb_cyl->setCurrentIndex(mat_cyl-mats->begin());
            if(mat_br!=mats->end())
                ui->cb_br->setCurrentIndex(mat_br-mats->begin());
            if(mat_tzp!=mats->end())
                ui->cb_tzp->setCurrentIndex(mat_tzp-mats->begin());
            if(mat_nozzle!=mats->end())
                ui->cb_nozzle->setCurrentIndex(mat_nozzle-mats->begin());
            if(fl_it!=fls->end())
                ui->cb_fuel->setCurrentIndex(fl_it-fls->begin());
        }
        else{
            ui->le_Hmax->clear();
            ui->le_Hmin->clear();
            ui->le_Xmax->clear();
            ui->le_Vtar->clear();
            ui->le_mstone->clear();
            ui->le_Pk->setText(QString("10"));
            ui->le_Pa->setText(QString("0.06"));
            ui->cb_cyl->setCurrentIndex(0);
            ui->cb_br->setCurrentIndex(0);
            ui->cb_tzp->setCurrentIndex(0);
            ui->cb_nozzle->setCurrentIndex(0);
            ui->cb_fuel->setCurrentIndex(0);
        }
    }
}



bool SetFlyTask::isDataValid()
{
    on_le_Hmax_editingFinished();
    on_le_Hmin_editingFinished();
    on_le_Pa_editingFinished();
    on_le_Pk_editingFinished();
    on_le_Vtar_editingFinished();
    on_le_Xmax_editingFinished();
    on_le_mstone_editingFinished();
    on_le_teth_editingFinished();
    return isHmaxValid&&
            isHminValid&&
            isPaValid&&
            isPkValid&&
            isVtargetValid&&
            isXmaxValid&&
            isMilestoneValid&&
            isTethValid;
}

void SetFlyTask::on_le_Hmax_editingFinished()
{
    isHmaxValid=true;
    double Hmax{0};
    if(ui->le_Hmax->text().isEmpty()|| (Hmax=ui->le_Hmax->text().toDouble())<=0){
        ui->label_warning->setText("параметр Hmax введен неверно.");
        ui->label_warning->show();
        isHmaxValid=false;
    }
}

void SetFlyTask::on_le_Hmin_editingFinished()
{
    isHminValid=true;
    double Hmin{0};
    if(ui->le_Hmin->text().isEmpty()|| (Hmin=ui->le_Hmin->text().toDouble())<=0){
        ui->label_warning->setText("параметр Hmin введен неверно.");
        ui->label_warning->show();
        isHminValid=false;
    }
}

void SetFlyTask::on_le_Xmax_editingFinished()
{
    isXmaxValid=true;
    double Xmax{0};
    if(ui->le_Xmax->text().isEmpty()|| (Xmax=ui->le_Xmax->text().toDouble())<=0){
        ui->label_warning->setText("параметр Xmax введен неверно.");
        ui->label_warning->show();
        isXmaxValid=false;
    }
}

void SetFlyTask::on_le_mstone_editingFinished()
{
    isMilestoneValid=true;
    double mstone{0};
    if(ui->le_mstone->text().isEmpty()|| (mstone=ui->le_mstone->text().toDouble())<=0){
        ui->label_warning->setText("параметр РВЗ введен неверно.");
        ui->label_warning->show();
        isMilestoneValid=false;
    }

}

void SetFlyTask::on_le_Vtar_editingFinished()
{
    isVtargetValid=true;
    double vtar{0};
    if(ui->le_Vtar->text().isEmpty()||(vtar=ui->le_Vtar->text().toDouble())<=0){
        ui->label_warning->setText("Скорость цели указана неверно.");
        ui->label_warning->show();
        isVtargetValid=false;
    }
}

void SetFlyTask::on_le_teth_editingFinished()
{
    isTethValid=true;
    double teth{0};
    if(ui->le_teth->text().isEmpty()||(teth=ui->le_teth->text().toDouble())<=0){
        ui->label_warning->setText("угол наклона тр-ии к ЛВЗ неверен");
        ui->label_warning->show();
        isTethValid=false;
    }
}

void SetFlyTask::on_le_Pa_editingFinished()
{
    isPaValid=true;
    double pa{0};
    if(ui->le_Pa->text().isEmpty()||(pa=ui->le_Pa->text().toDouble())<=0){
        ui->label_warning->setText("Давление на срезе сопла неверно");
        ui->label_warning->show();
        isPaValid=false;
    }
}

void SetFlyTask::on_le_Pk_editingFinished()
{
    isPkValid=true;
    double pk{0};
    if(ui->le_Pk->text().isEmpty()||(pk=ui->le_Pk->text().toDouble())<=0){
        ui->label_warning->setText("Давление в кр.с. сопла неверно");
        ui->label_warning->show();
        isPkValid=false;
    }

}

void SetFlyTask::on_buttonBox_accepted()
{
    if(isDataValid() && bcmodel && mats && fls){

        auto mat_cyl=std::find_if(mats->begin(),mats->end(),[&](const material& m){
            return m.name==ui->cb_cyl->currentText().toStdString();});
        auto mat_br=std::find_if(mats->begin(),mats->end(),[&](const material& m){
            return m.name==ui->cb_br->currentText().toStdString();});
        auto mat_tzp=std::find_if(mats->begin(),mats->end(),[&](const material& m){
            return m.name==ui->cb_tzp->currentText().toStdString();});
        auto mat_nozzle=std::find_if(mats->begin(),mats->end(),[&](const material& m){
            return m.name==ui->cb_nozzle->currentText().toStdString();});
        auto fuel_iterator=std::find_if(fls->begin(),fls->end(),[&](const fuel& m){
            return m.name==ui->cb_fuel->currentText().toStdString();});

        if(mat_cyl!=mats->end() &&
                mat_br!=mats->end()&&
                mat_tzp!=mats->end()&&
                mat_nozzle!=mats->end()&&
                fuel_iterator!=fls->end()){
            try{
                bcmodel->setInputData(
                            InputData(
                                ui->le_Hmax->text().toDouble(),
                                ui->le_Hmin->text().toDouble(),
                                ui->le_Xmax->text().toDouble(),
                                ui->le_Vtar->text().toDouble(),
                                ui->le_mstone->text().toDouble(),
                                *mat_cyl,*mat_nozzle,*mat_br,*mat_tzp,
                                *fuel_iterator,
                                ui->le_teth->text().toDouble(),
                                ui->le_Pk->text().toDouble(),
                                ui->le_Pa->text().toDouble()
                                ));
                accept();
            }
            catch(const InputData::IDexception& idex){
                ui->label_warning->setText("Введенные данные некорректны.");
                ui->label_warning->show();
            }
            catch(std::exception& e){
                ui->label_warning->setText(QString::fromStdString(e.what())+=" /input data exception");
                ui->label_warning->show();
            }
            catch(...){
                ui->label_warning->setText("some input data exception");
                ui->label_warning->show();
            }

        }
        else throw std::logic_error("setflytask:combobox_error");
    }



}
