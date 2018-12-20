#include "workwindow.h"
#include "ui_workwindow.h"
#include <QStandardItem>
#include <QPair>
#include <QVariant>
#include <fstream>
#include "Dialogs/errordialog.h"
#include <QMessageBox>


WorkWindow::WorkWindow(QString matfile, QString hfuelsfile, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WorkWindow)
{
    ui->setupUi(this);


    material corp("orpl",7810,1860);
    material tzp("tzp",1600,1600);
    material nozzle("steel",4700,4700);
    material br("br",1300,1300);
    fuel fl("fuel",15,2460,1800,3300,290,1.16,5.75,0.4);
    double Dmax=0.255;
    InputData indat(Dmax,40000,1000,120000,350,1500,corp,nozzle,br,tzp,fl,10,0.06);
    ballisticCalculator* rc=new ballisticCalculator(
                indat,
                std::make_unique<RocketModel>(Dmax,true),
                std::make_unique<fInterpAtmosphere>("1.txt")
                );


    //register the model
    auto bc=new balcalcItemModel(std::unique_ptr<ballisticCalculator>(rc));
    ui->treeView->setModel(bc);
    updateActions();
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeView->setAutoExpandDelay(0);
    connect(bc,SIGNAL(updated()),ui->treeView,SLOT(expandAll()));



    //проверка
    bc->setNosecone(br,0.19,0.5,0.05);
    bc->addConoid(nozzle,0.2,0.22,0.5,0.05);
    bc->addConoid(nozzle,0.22,0.24,0.7,0.05);
    bc->addConoid(corp,0.24,0.255,0.1,0.05);
    bc->addEquipment("gc",1.0,25);
    bc->addEquipment("bch",1.6,45);
    bc->addplane(nozzle,1.05,0.25,0.15,0.03,0.01,0.1,0.01,0.01,0.01,0.01,0.15);
    bc->setTailStab(nozzle,0.45,0.25,0.05,0.02,0.15,0.05,0.05,0.05,0.05,0.35);


    if(!readMaterials(matfile)){
        errd= new errorDialog(QString("Ошибка: Файл %1 %2").arg(matfile).arg(" поврежден."),this);
        errd->show();
    }
    if(!readHardfuels(hfuelsfile)){
        errd=new errorDialog(QString("Ошибка: Файл %1 %2").arg(hfuelsfile).arg(" поврежден."),this);
        errd->show();
    }

    addconedial=new addConoidDialog(&materials,bc,this);
    ncdial=new SetNoseConeDialog(&materials,bc,this);
}

WorkWindow::~WorkWindow()
{
    delete ui;
    delete addconedial;
    delete errd;
}



void WorkWindow::on_pushButton_clicked()
{
}

void WorkWindow::insertChild()
{

}

void WorkWindow::selectionChangedSlot(const QItemSelection &, const QItemSelection &)
{
    std::cout<<"selch"<<std::endl;
}

void WorkWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    /*constexpr int rocketid=1,
nconeid{10},
tailstabid{11},
stabsid{12},
conesid{13},equipsid{14};*/




    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    if(!index.isValid())return;
    curindex=static_cast<size_t>(index.row());
    QMenu menu(this);
    int id=index.data(Qt::UserRole+1).toInt();
    switch(id){
        case int(balcalcItem::itemtype::Project):{
            QAction *newActc = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Новый"), this);
            newActc->setStatusTip(tr("create"));
            connect(newActc, SIGNAL(triggered()), this, SLOT(insertChild()));
            menu.addAction(newActc);
            break;
        }
        case int(balcalcItem::itemtype::FlyTask):{
            QAction *newActr = new QAction(tr("&Редактировать"), this);
            newActr->setStatusTip(tr("edit"));
            connect(newActr, SIGNAL(triggered()), this, SLOT(insertChild()));
            menu.addAction(newActr);
            break;
        }

        case int(balcalcItem::itemtype::TailStabCont):{
            QAction *newActs = new QAction(tr("&Установить"), this);
            newActs->setStatusTip(tr("set"));
            connect(newActs, SIGNAL(triggered()), this, SLOT(insertChild()));
            menu.addAction(newActs);
            break;
        }
        case int(balcalcItem::itemtype::TailStab):{
            QAction *newActs = new QAction(tr("&Изменить"), this);
            newActs->setStatusTip(tr("set"));
            connect(newActs, SIGNAL(triggered()), this, SLOT(insertChild()));
            menu.addAction(newActs);

            QAction *newActs2 = new QAction(tr("&Удалить"), this);
            newActs->setStatusTip(tr("del"));
            connect(newActs2, SIGNAL(triggered()), this, SLOT(insertChild()));
            menu.addAction(newActs2);
            break;
        }

        case int(balcalcItem::itemtype::NoseconeCont):{
            QAction *newActs = new QAction(tr("&Установить"), this);
            newActs->setStatusTip(tr("set"));
            connect(newActs, SIGNAL(triggered()), this, SLOT(SetNoseDialog()));
            menu.addAction(newActs);
            break;
        }
        case int(balcalcItem::itemtype::Nosecone):{
            QAction *newActs = new QAction(tr("&Изменить"), this);
            newActs->setStatusTip(tr("edit"));
            connect(newActs, SIGNAL(triggered()), this, SLOT(EditNoseDialog()));
            menu.addAction(newActs);
            break;
        }

        case int(balcalcItem::itemtype::ConesCont):{
            QAction *newActa = new QAction(tr("&Добавить"), this);
            newActa->setStatusTip(tr("add"));
            connect(newActa, SIGNAL(triggered()), this, SLOT(AddConDialog()));
            menu.addAction(newActa);
            break;
        }
        case int(balcalcItem::itemtype::Cone):{
            QAction *newActa = new QAction(tr("&Редактировать"), this);
            newActa->setStatusTip(tr("edit"));
            connect(newActa, SIGNAL(triggered()), this, SLOT(EditConDialog()));
            menu.addAction(newActa);
            QAction *newActa2 = new QAction(tr("&Удалить"), this);
            newActa->setStatusTip(tr("del"));
            connect(newActa2, SIGNAL(triggered()), this, SLOT(RemoveConoid()));
            menu.addAction(newActa2);
            break;
        }

        case int(balcalcItem::itemtype::PlanesCont):{
            break;
        }
        case int(balcalcItem::itemtype::Plane):{
            break;
        }
        case int(balcalcItem::itemtype::EquipsCont):{
            break;
        }
        case int(balcalcItem::itemtype::Equipment):{
            break;
        }

    }
    menu.exec(ui->treeView->mapToGlobal(pos));
}




void WorkWindow::updateActions(const QItemSelection &selected,const QItemSelection &deselected) {
    updateActions();
}

void WorkWindow::AddConDialog()
{
    addconedial->setdata();
    addconedial->show();
}

void WorkWindow::SetNoseDialog()
{
    ncdial->setdata();
    ncdial->show();
}

void WorkWindow::EditNoseDialog()
{
    ncdial->setdata(true);
    ncdial->show();
}

void WorkWindow::EditConDialog(){
    addconedial->setdata(curindex);
    addconedial->show();
}

void WorkWindow::RemoveConoid(){
    QMessageBox msgBox(this);
    msgBox.setInformativeText("Вы действительно хотите удалить данный отсек?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    //поправить вид диалога
    int ret = msgBox.exec();
    if(ret==QMessageBox::Ok){
        balcalcItemModel* model=dynamic_cast<balcalcItemModel*>(ui->treeView->model());
        if(model && curindex<model->modelstate().at(2)>curindex)
            model->ejectConoid(curindex);
    }
}

void WorkWindow::updateActions() {
    bool hasSelection = !ui->treeView->selectionModel()->selection().isEmpty();
    //  ui->treeView->setEnabled(hasSelection);
    // ui->treeView->setEnabled(hasSelection);
    bool hasCurrent = ui->treeView->selectionModel()->currentIndex().isValid();
    //  ui->treeView->setEnabled(hasCurrent);
    //  ui->treeView->setEnabled(hasCurrent);
    if (hasCurrent) {
        ui->treeView->closePersistentEditor(ui->treeView->selectionModel()->currentIndex());

        int row = ui->treeView->selectionModel()->currentIndex().row();
        int column = ui->treeView->selectionModel()->currentIndex().column();
        if (ui->treeView->selectionModel()->currentIndex().parent().isValid())
            statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
        else
            statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
    }
}

bool WorkWindow::readMaterials(QString matfile)
{
    std::ifstream in(matfile.toStdString());
    if(!in)return false;
    std::cout<<"reading mat"<<std::endl;
    material mat;
    while(in>>mat){
        materials.push_back(mat);
        std::cout<<mat.name<<std::endl;
    }
    std::cout<<in.eof()<<std::endl;
    if(in.eof())return true;
    return false;
}

bool WorkWindow::readHardfuels(QString hfuelsfile)
{
    std::ifstream in(hfuelsfile.toStdString());
    if(!in)return false;
    fuel fl;
    std::cout<<"reading fuel"<<std::endl;
    while(in>>fl)
        if(in)hardfuels.push_back(fl);
    if(in.eof())return true;
    return false;
}

