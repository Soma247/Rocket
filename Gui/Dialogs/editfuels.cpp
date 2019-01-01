#include "editfuels.h"
#include "ui_editfuels.h"

#include <QMessageBox>

EditFuels::EditFuels(std::vector<material> *materials, std::vector<fuel> *fuels, QWidget *parent) :
                                   QDialog(parent),ui(new Ui::EditFuels),mats{materials},fls{fuels}{

    ui->setupUi(this);
    ui->label_warning_f->hide();
    ui->label_warning_m->hide();
    if(mats&&fls&&mats->size()&&fls->size()){

        for(const material& m:*mats)
            ui->cb_mats->addItem(QString::fromStdString(m.name));
        for(const fuel& f:*fls)
            ui->cb_fuels->addItem(QString::fromStdString(f.name));

        ui->le_fl_Pudst->setText(QString("%1").arg(fls->front().Prelst));
        ui->le_fl_Rst->setText(QString("%1").arg(fls->front().rst));
        ui->le_fl_Tst->setText(QString("%1").arg(fls->front().tst));
        ui->le_fl_al->setText(QString("%1").arg(fls->front().AL));
        ui->le_fl_k->setText(QString("%1").arg(fls->front().kst));
        ui->le_fl_name->setText(QString::fromStdString(fls->front().name));
        ui->le_fl_ro->setText(QString("%1").arg(fls->front().rot));
        ui->le_fl_ukoef->setText(QString("%1").arg(fls->front().ukoef));
        ui->le_fl_upow->setText(QString("%1").arg(fls->front().upow));

        ui->le_mat_name->setText(QString::fromStdString(mats->front().name));
        ui->le_mat_ro->setText(QString("%1").arg(mats->front().Ro));
        ui->le_mat_sigma->setText(QString("%1").arg(mats->front().SigmaV));
        connect(this,SIGNAL(matupdated()),this,SLOT(update()));
        connect(this,SIGNAL(flupdated()),this,SLOT(update()));
    }
    else{
        ui->label_warning_f->setText("Critical error, haven't data.");
        ui->label_warning_f->show();
    }

}

EditFuels::~EditFuels()
{
    delete ui;
}

void EditFuels::on_cb_fuels_currentIndexChanged(const QString &){
  /*  auto mat_iterator=std::find_if(mats->begin(),mats->end(),[&](const material& m){
        return m.name==ui->cb_mats->currentText().toStdString();});*/
    auto fuel_iterator=std::find_if(fls->begin(),fls->end(),[&](const fuel& m){
        return m.name==ui->cb_fuels->currentText().toStdString();});

    if(fuel_iterator!=fls->end()){
        ui->cb_fuels->addItem(QString::fromStdString(fuel_iterator->name));
    ui->le_fl_Pudst->setText(QString("%1").arg(fuel_iterator->Prelst));
    ui->le_fl_Rst->setText(QString("%1").arg(fuel_iterator->rst));
    ui->le_fl_Tst->setText(QString("%1").arg(fuel_iterator->tst));
    ui->le_fl_al->setText(QString("%1").arg(fuel_iterator->AL));
    ui->le_fl_k->setText(QString("%1").arg(fuel_iterator->kst));
    ui->le_fl_name->setText(QString::fromStdString(fuel_iterator->name));
    ui->le_fl_ro->setText(QString("%1").arg(fuel_iterator->rot));
    ui->le_fl_ukoef->setText(QString("%1").arg(fuel_iterator->ukoef));
    ui->le_fl_upow->setText(QString("%1").arg(fuel_iterator->upow));
    }
}

void EditFuels::on_cb_mats_currentIndexChanged(const QString &)
{
    auto mat_iterator=std::find_if(mats->begin(),mats->end(),[&](const material& m){
          return m.name==ui->cb_mats->currentText().toStdString();});
    if(mat_iterator!=mats->end()){
        ui->le_mat_name->setText(QString::fromStdString(mat_iterator->name));
        ui->le_mat_ro->setText(QString("%1").arg(mat_iterator->Ro));
        ui->le_mat_sigma->setText(QString("%1").arg(mat_iterator->SigmaV));
    }

}

void EditFuels::on_le_mat_name_editingFinished()
{
    ismat_nameValid=true;
    if(ui->le_mat_name->text().isEmpty()){
        ui->label_warning_m->setText("введите имя материала");
        ui->label_warning_m->show();
        ismat_nameValid=false;
    }
}

void EditFuels::on_le_mat_ro_editingFinished()
{
    ismat_roValid=true;
    double mat_ro{0};
    if(ui->le_mat_ro->text().isEmpty()|| (mat_ro=ui->le_mat_ro->text().toDouble())<=0){
        ui->label_warning_m->setText("плотность материала введена неверно.");
        ui->label_warning_m->show();
        ismat_roValid=false;
    }
}

void EditFuels::on_le_mat_sigma_editingFinished()
{
    ismat_sigmaValid=true;
    double mat_sigma{0};
    if(ui->le_mat_ro->text().isEmpty()|| (mat_sigma=ui->le_mat_ro->text().toDouble())<=0){
        ui->label_warning_m->setText("уд. прочность материала введена неверно.");
        ui->label_warning_m->show();
        ismat_sigmaValid=false;
    }

}

void EditFuels::on_le_fl_name_editingFinished()
{
    isfl_nameValid=true;
    if(ui->le_fl_name->text().isEmpty()){
        ui->label_warning_f->setText("введите имя топлива");
        ui->label_warning_f->show();
        isfl_nameValid=false;
    }

}

void EditFuels::on_le_fl_al_editingFinished()
{
    isfl_alValid=true;
    double fl_al{0};
    if(ui->le_fl_al->text().isEmpty()|| (fl_al=ui->le_fl_al->text().toDouble())<=0||fl_al>30){
        ui->label_warning_f->setText("процентное содержание AL введено неверно.");
        ui->label_warning_f->show();
        isfl_alValid=false;
    }

}

void EditFuels::on_le_fl_Pudst_editingFinished()
{
    isfl_PudstValid=true;
    double fl_Pudst{0};
    if(ui->le_fl_Pudst->text().isEmpty()|| (fl_Pudst=ui->le_fl_Pudst->text().toDouble())<1000||fl_Pudst>10000){
        ui->label_warning_f->setText("Удельный импульс тяги введен неверно.");
        ui->label_warning_f->show();
        isfl_PudstValid=false;
    }
}

void EditFuels::on_le_fl_ro_editingFinished()
{
    isfl_roValid=true;
    double fl_ro{0};
    if(ui->le_fl_ro->text().isEmpty()|| (fl_ro=ui->le_fl_ro->text().toDouble())<500||fl_ro>3000){
        ui->label_warning_f->setText("плотность топлива введена неверно.");
        ui->label_warning_f->show();
        isfl_roValid=false;
    }

}

void EditFuels::on_le_fl_Tst_editingFinished()
{
    isfl_TstValid=true;
    double fl_Tst{0};
    if(ui->le_fl_Tst->text().isEmpty()|| (fl_Tst=ui->le_fl_ro->text().toDouble())<600||fl_Tst>5000){
        ui->label_warning_f->setText("температура горения топлива введена неверно.");
        ui->label_warning_f->show();
        isfl_TstValid=false;
    }
}

void EditFuels::on_le_fl_Rst_editingFinished()
{
    isfl_RstValid=true;
    double fl_Rst{0};
    if(ui->le_fl_Rst->text().isEmpty()|| (fl_Rst=ui->le_fl_Rst->text().toDouble())<50||fl_Rst>1000){
        ui->label_warning_f->setText("газовая постоянная введена неверно.");
        ui->label_warning_f->show();
        isfl_RstValid=false;
    }

}

void EditFuels::on_le_fl_k_editingFinished()
{
    isfl_kValid=true;
    double fl_k{0};
    if(ui->le_fl_k->text().isEmpty()|| (fl_k=ui->le_fl_k->text().toDouble())<0.5||fl_k>4){
        ui->label_warning_f->setText("показатель адиабаты введен неверно.");
        ui->label_warning_f->show();
        isfl_kValid=false;
    }

}

void EditFuels::on_le_fl_ukoef_editingFinished()
{
    isfl_ukoefValid=true;
    double fl_ukoef{0};
    if(ui->le_fl_ukoef->text().isEmpty()|| (fl_ukoef=ui->le_fl_ukoef->text().toDouble())<0.5||fl_ukoef>8){
        ui->label_warning_f->setText("коэфф. скор. горения введен неверно.");
        ui->label_warning_f->show();
        isfl_ukoefValid=false;
    }

}

void EditFuels::on_le_fl_upow_editingFinished()
{
    is_fl_upowValid=true;
    double fl_upow{0};
    if(ui->le_fl_upow->text().isEmpty()|| (fl_upow=ui->le_fl_upow->text().toDouble())<0.1||fl_upow>1){
        ui->label_warning_f->setText("степень скор. горения введена неверно.");
        ui->label_warning_f->show();
        is_fl_upowValid=false;
    }
}

void EditFuels::on_btn_savemat_clicked()
{
    on_le_mat_name_editingFinished();
    on_le_mat_ro_editingFinished();
    on_le_mat_sigma_editingFinished();
    if(matiscorrect()){
        auto mat_iterator=std::find_if(mats->begin(),mats->end(),[&](const material& m){
              return m.name==ui->le_mat_name->text().toStdString();});
        if(mat_iterator!=mats->end()){//если существует такой
            QMessageBox msgBox(this);
            msgBox.setStyleSheet(ui->le_mat_sigma->styleSheet());
            msgBox.setInformativeText("Вы действительно хотите изменить этот материал?");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            //поправить вид диалога
            int ret = msgBox.exec();
            if(ret==QMessageBox::Ok){
                *mat_iterator=material(ui->le_mat_name->text().toStdString(),
                                       ui->le_mat_ro->text().toDouble(),
                                       ui->le_mat_sigma->text().toDouble());
                //обновить все окна, исп материал и сохранить в файл
            }
        }
        else{
            mats->push_back(material(ui->le_mat_name->text().toStdString(),
                                   ui->le_mat_ro->text().toDouble(),
                                   ui->le_mat_sigma->text().toDouble()));
            //обновить все окна, исп материал и сохранить в файл
        }
        emit matupdated();
    }
}

void EditFuels::on_btn_savefl_clicked()
{
    on_le_mat_name_editingFinished();
    on_le_mat_ro_editingFinished();
    on_le_mat_sigma_editingFinished();
    if(matiscorrect()){
        auto mat_iterator=std::find_if(mats->begin(),mats->end(),[&](const material& m){
              return m.name==ui->le_mat_name->text().toStdString();});
        if(mat_iterator!=mats->end()){//если существует такой
            QMessageBox msgBox(this);
            msgBox.setStyleSheet(ui->le_mat_sigma->styleSheet());
            msgBox.setInformativeText("Вы действительно хотите изменить этот материал?");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            //поправить вид диалога
            int ret = msgBox.exec();
            if(ret==QMessageBox::Ok){
                *mat_iterator=material(ui->le_mat_name->text().toStdString(),
                                       ui->le_mat_ro->text().toDouble(),
                                       ui->le_mat_sigma->text().toDouble());
                //обновить все окна, исп материал и сохранить в файл
            }
        }
        else{
            mats->push_back(material(ui->le_mat_name->text().toStdString(),
                                   ui->le_mat_ro->text().toDouble(),
                                   ui->le_mat_sigma->text().toDouble()));
            //обновить все окна, исп материал и сохранить в файл
        }
        emit flupdated();
    }

}

bool EditFuels::matiscorrect(){
    return   ismat_nameValid&& ismat_roValid&& ismat_sigmaValid;
}

bool EditFuels::fliscorrect(){
    return isfl_PudstValid&&isfl_RstValid&&
            isfl_TstValid&&isfl_alValid&&
            isfl_kValid&&isfl_nameValid&&
            isfl_roValid&&isfl_ukoefValid&&
            is_fl_upowValid;
}

void EditFuels::on_pushButton_3_clicked()
{
    accept();
}

void EditFuels::update()
{
    ui->cb_mats->clear();
    ui->cb_fuels->clear();
    if(mats&&fls&&mats->size()&&fls->size()){
        for(const material& m:*mats)
            ui->cb_mats->addItem(QString::fromStdString(m.name));
        for(const fuel& f:*fls)
            ui->cb_fuels->addItem(QString::fromStdString(f.name));
    }
    else{
        ui->label_warning_f->setText("Critical error, haven't data.");
        ui->label_warning_f->show();
    }
}
