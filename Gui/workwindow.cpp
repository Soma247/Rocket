#include "workwindow.h"
#include "ui_workwindow.h"
#include <QStandardItem>
#include <QPair>
#include <QVariant>


WorkWindow::WorkWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WorkWindow)
{
    ui->setupUi(this);


    matherial corp("orpl",7810,1860);
    matherial tzp("tzp",1600,1600);
    matherial nozzle("steel",4700,4700);
    matherial br("br",1300,1300);
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

    addconedial=new addConoidDialog(bc,this);
    ncdial=new SetNoseConeDialog(bc,this);

    connect(bc,SIGNAL(updated()),ui->treeView,SLOT(expandAll()));


    //проверка
    bc->setNosecone(br,0.19,0.5,0.05);
    bc->addConoid(nozzle,0.2,0.22,0.5,0.05);
    bc->addConoid(nozzle,0.22,0.24,0.7,0.05);
    bc->addConoid(nozzle,0.24,0.255,0.1,0.05);
    bc->addEquipment("gc",1.0,25);
    bc->addEquipment("bch",1.6,45);
    bc->addplane(nozzle,1.05,0.25,0.15,0.03,0.01,0.1,0.01,0.01,0.01,0.01,0.15);
    bc->setTailStab(nozzle,0.45,0.25,0.05,0.02,0.15,0.05,0.05,0.05,0.05,0.35);

}

WorkWindow::~WorkWindow()
{
    delete ui;
    delete addconedial;
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
    std::cerr<<index.data(Qt::UserRole+1).toInt()<<std::endl;
    QMenu menu(this);
    int id=index.data(Qt::UserRole+1).toInt();
    if(index.data(Qt::UserRole+1).toInt()==int(balcalcItem::itemtype::Project)){
        QAction *newActc = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Создать"), this);
        newActc->setStatusTip(tr("create"));
        connect(newActc, SIGNAL(triggered()), this, SLOT(insertChild()));
        menu.addAction(newActc);
    }
    else if(id==int(balcalcItem::itemtype::FlyTask)){
        QAction *newActr = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Редактировать"), this);
        newActr->setStatusTip(tr("edit"));
        connect(newActr, SIGNAL(triggered()), this, SLOT(insertChild()));
        menu.addAction(newActr);
    }
    else if(id==int(balcalcItem::itemtype::TailStabCont)){
            QAction *newActs = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Установить"), this);
            newActs->setStatusTip(tr("set"));
            connect(newActs, SIGNAL(triggered()), this, SLOT(insertChild()));
            menu.addAction(newActs);
    }
    else if(id==int(balcalcItem::itemtype::NoseconeCont)){
        QAction *newActs = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Установить"), this);
        newActs->setStatusTip(tr("set"));
        connect(newActs, SIGNAL(triggered()), this, SLOT(SetNoseDialog()));
        menu.addAction(newActs);
    }
    else if(id==int(balcalcItem::itemtype::ConesCont)||id==int(balcalcItem::itemtype::PlanesCont)||id==int(balcalcItem::itemtype::EquipsCont)){
        QAction *newActa = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Добавить"), this);
        newActa->setStatusTip(tr("add"));
        connect(newActa, SIGNAL(triggered()), this, SLOT(AddConDialog()));
        menu.addAction(newActa);
    }
    menu.exec(ui->treeView->mapToGlobal(pos));
}




void WorkWindow::updateActions(const QItemSelection &selected,const QItemSelection &deselected) {
    updateActions();
}

void WorkWindow::AddConDialog()
{
    addconedial->show();
}

void WorkWindow::SetNoseDialog()
{
    ncdial->show();
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

