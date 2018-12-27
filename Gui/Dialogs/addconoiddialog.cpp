#include "addconoiddialog.h"
#include "ui_addconoiddialog.h"

addConoidDialog::addConoidDialog(const std::vector<material> *materials, balcalcItemModel* model, QWidget *parent) :
    QDialog(parent),ui(new Ui::addConoidDialog),bcmodel{model}
{
    ui->setupUi(this);
    mats=materials;
}

addConoidDialog::~addConoidDialog()
{
    delete ui;
}

void addConoidDialog::setdata(size_t index){
    std::cerr<<"setdata :"<<index<<std::endl;
        ui->label_warning->hide();
        isD1valid=isD2valid=isLvalid=isdeltvalid=false;
        curindex=index;
        if(mats){
            ui->matComboBox->clear();
        for(const material& m:*mats)
            ui->matComboBox->addItem(QString::fromStdString(m.name));
        }
    if(curindex==overmaxindex){
        ui->le_D1->clear();
        ui->le_D2->clear();
        ui->le_L->clear();
        ui->le_delt->clear();
    }
    else if(bcmodel){
            ui->le_D1->setText(QString("%1").arg(bcmodel->getheaddata().conespar.at(index).dbeg));
            ui->le_D2->setText(QString("%1").arg(bcmodel->getheaddata().conespar.at(index).dend));
            ui->le_L->setText(QString("%1").arg(bcmodel->getheaddata().conespar.at(index).len));
            ui->le_delt->setText(QString("%1").arg(bcmodel->getheaddata().conespar.at(index).delt));
            auto it=std::find(mats->begin(),mats->end(),bcmodel->getheaddata().conespar.at(index).mat);
            if(it!=mats->end()){
                ui->matComboBox->setCurrentIndex(it-mats->begin());
            }
    }
}

void addConoidDialog::on_buttonBox_accepted()
{
    std::cerr<<isDataValid()<<" "<<bool(bcmodel)<<" "<<bool(mats)<<std::endl;
    if(isDataValid() && bcmodel && mats){
  std::cerr<<"data model mats valid;"<<std::endl;

        auto it=std::find_if(mats->begin(),mats->end(),
                             [&](const material& m){
            return m.name==ui->matComboBox->currentText().toStdString();});
        if(it!=mats->end()){
            if(curindex==overmaxindex){
                  std::cerr<<"curindex==overmax"<<curindex<<std::endl;
                bcmodel->addConoid(*it,ui->le_D1->text().toDouble(),
                                   ui->le_D2->text().toDouble(),
                                   ui->le_L->text().toDouble(),
                                   ui->le_delt->text().toDouble());
            }
            else{

                std::cout<<"edit"<<std::endl;
                bcmodel->ejectConoid(curindex);
                bcmodel->insertConoid(*it,ui->le_D1->text().toDouble(),
                                      ui->le_D2->text().toDouble(),
                                      ui->le_L->text().toDouble(),
                                      ui->le_delt->text().toDouble(),curindex);
            }
        }
    }
}



void addConoidDialog::on_le_D1_editingFinished(){
    ui->label_warning->hide();
    checkD1();
}

void addConoidDialog::on_le_D2_editingFinished(){
    ui->label_warning->hide();
    checkD2();
}

void addConoidDialog::on_le_L_editingFinished(){
    ui->label_warning->hide();
    checkL();
}

void addConoidDialog::on_le_delt_editingFinished(){
    ui->label_warning->hide();
    checkdelt();
}

void addConoidDialog::checkD1(){
    isD1valid=true;
    double D1{0};
    if(ui->le_D1->text().isEmpty()|| (D1=ui->le_D1->text().toDouble())<=0){
        ui->label_warning->setText("параметр d1 введен неверно.");
        ui->label_warning->show();
        isD1valid=false;
    }
}

void addConoidDialog::checkD2(){
    isD2valid=true;
    double D2{0};
    if(ui->le_D2->text().isEmpty()|| (D2=ui->le_D2->text().toDouble())<=0){
        ui->label_warning->setText("параметр D2 введен неверно.");
        ui->label_warning->show();
        isD2valid=false;
    }
}

void addConoidDialog::checkL(){
    isLvalid=true;
    double L{0};
    if(ui->le_L->text().isEmpty()|| (L=ui->le_L->text().toDouble())<=0){
        ui->label_warning->setText("параметр L введен неверно.");
        ui->label_warning->show();
        isLvalid=false;
    }
}

void addConoidDialog::checkdelt(){
    isdeltvalid=true;
    double delt{0};
    if(ui->le_delt->text().isEmpty()|| (delt=ui->le_delt->text().toDouble())<=0||
           ui->le_delt->text().toDouble()>0.5*ui->le_D1->text().toDouble()||
           ui->le_delt->text().toDouble()>0.5*ui->le_D2->text().toDouble()){
        ui->label_warning->setText("параметр delt введен неверно.");
        ui->label_warning->show();
        isdeltvalid=false;
    }
}

bool addConoidDialog::isDataValid(){
    if(curindex!=overmaxindex){
      checkD1();
      checkD2();
      checkL();
      checkdelt();
    }
    return isD1valid && isD2valid && isLvalid && isdeltvalid;
}
