#include "workwindow.h"
#include "ui_workwindow.h"
#include <QStandardItem>
#include <QPair>
#include <QVariant>


constexpr int rocketid=1,flytaskid{2},
nconeid{10},
tailstabid{11},
stabsid{12},
conesid{13},equipsid{14};

WorkWindow::WorkWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WorkWindow)
{
    ui->setupUi(this);
      auto standardModel = new QStandardItemModel ;
      QStandardItem *rootNode = standardModel->invisibleRootItem();
    QIcon ic=QIcon("resources/cont_red_circle.png");

      //defining a couple of items
      QStandardItem *project =new QStandardItem("Проект");
      project->setData(QVariant(0));

      QStandardItem *rocket =new QStandardItem(ic,"Модель ракеты");
      rocket->setData(QVariant(rocketid));

          QStandardItem *nosecone =new QStandardItem(ic,"Носовой обтекатель");
          nosecone->setData(QVariant(nconeid));
          QStandardItem *tailstab =new QStandardItem("Хвостовой стабилизатор");
          tailstab->setData(QVariant(tailstabid));
          QStandardItem *stabs =new QStandardItem("Несущие плоскости");
          stabs->setData(QVariant(stabsid));
          QStandardItem *cones =new QStandardItem("Отсеки");
          cones->setData(QVariant(conesid));
          QStandardItem *equips =new QStandardItem("Аппаратура");
          equips->setData(QVariant(equipsid));

      QStandardItem *flytask =new QStandardItem("Программа полета");
      flytask->setData(QVariant(flytaskid));

      //building up the hierarchy
      rootNode->appendRow(project);
      project->appendRow(rocket);
      project->appendRow(flytask);
      rocket->appendRow(nosecone);
      rocket->appendRow(tailstab);
      rocket->appendRow(stabs);
      rocket->appendRow(cones);
      rocket->appendRow(equips);


      //register the model
      ui->treeView->setModel(standardModel);
      ui->treeView->expandAll();
      //selection changes shall trigger a slot
      //QItemSelectionModel *selectionModel= ui->treeView->selectionModel();
      //connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

      connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
              this, &WorkWindow::updateActions);


      updateActions();
      ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
}

WorkWindow::~WorkWindow()
{
    delete ui;
}

void WorkWindow::updateActions()
{
    bool hasSelection = !ui->treeView->selectionModel()->selection().isEmpty();


    bool hasCurrent = ui->treeView->selectionModel()->currentIndex().isValid();

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

void WorkWindow::on_pushButton_clicked()
{
    std::cerr<<"!"<<std::endl;
}

void WorkWindow::insertChild()
{

}

void WorkWindow::selectionChangedSlot(const QItemSelection &, const QItemSelection &)
{

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
    if(index.data(Qt::UserRole+1).toInt()==rocketid){
        QAction *newActc = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Создать"), this);
        newActc->setStatusTip(tr("create"));
        connect(newActc, SIGNAL(triggered()), this, SLOT(insertChild()));
        menu.addAction(newActc);
    }
    else if(id==flytaskid){
        QAction *newActr = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Редактировать"), this);
        newActr->setStatusTip(tr("edit"));
        connect(newActr, SIGNAL(triggered()), this, SLOT(insertChild()));
        menu.addAction(newActr);
    }
    else if(id==nconeid || id==tailstabid){
            QAction *newActs = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Установить"), this);
            newActs->setStatusTip(tr("set"));
            connect(newActs, SIGNAL(triggered()), this, SLOT(insertChild()));
            menu.addAction(newActs);
    }
    else if(id==conesid||id==stabsid||id==equipsid){
        QAction *newActa = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Добавить"), this);
        newActa->setStatusTip(tr("add"));
        connect(newActa, SIGNAL(triggered()), this, SLOT(insertChild()));
        menu.addAction(newActa);
    }
    menu.exec(ui->treeView->mapToGlobal(pos));
}
