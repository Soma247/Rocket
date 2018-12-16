#include "setnoseconedialog.h"
#include "ui_setnoseconedialog.h"

SetNoseConeDialog::SetNoseConeDialog(balcalcItemModel* model,QWidget *parent) :
    QDialog(parent),bcmodel{model},
    ui(new Ui::SetNoseConeDialog)
{
    ui->setupUi(this);
}

SetNoseConeDialog::~SetNoseConeDialog()
{
    delete ui;
}

void SetNoseConeDialog::on_SetNoseConeDialog_accepted()
{
    //fix it
    if(bcmodel)bcmodel->setNosecone(matherial("mat",1400,1400),0.5,0.5,0.3);
}
