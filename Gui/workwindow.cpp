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
      auto standardModel = new QStandardItemModel ;
      QStandardItem *rootNode = standardModel->invisibleRootItem();

      //defining a couple of items
      QStandardItem *project =new QStandardItem("Проект");
      project->setData(QVariant(0));

      QStandardItem *rocket =new QStandardItem("Модель ракеты");
      rocket->setData(QVariant(1));

          QStandardItem *nosecone =new QStandardItem("Носовой обтекатель");
          nosecone->setData(QVariant(10));
          QStandardItem *tailstab =new QStandardItem("Хвостовой стабилизатор");
          tailstab->setData(QVariant(11));
          QStandardItem *stabs =new QStandardItem("Несущие плоскости");
          stabs->setData(QVariant(12));
          QStandardItem *cones =new QStandardItem("Отсеки");
          cones->setData(QVariant(13));
          QStandardItem *equips =new QStandardItem("Аппаратура");
          equips->setData(QVariant(14));

      QStandardItem *flytask =new QStandardItem("Программа полета");
      flytask->setData(QVariant(2));

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

    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    std::cerr<<index.data(Qt::UserRole+1).toInt()<<std::endl;
    QMenu menu(this);
    if(index.data(Qt::UserRole+1).toInt()<2){

    }
    else if(index.data(Qt::UserRole+1).toInt()==2){
        QAction *newActr = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Редактировать"), this);
        newActr->setStatusTip(tr("new sth"));
        connect(newActr, SIGNAL(triggered()), this, SLOT(insertChild()));
        menu.addAction(newActr);
    }
    else if(index.data(Qt::UserRole+1).toInt()==10 || index.data(Qt::UserRole+1).toInt()==11){
            QAction *newActs = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Установить"), this);
            newActs->setStatusTip(tr("new sth"));
            connect(newActs, SIGNAL(triggered()), this, SLOT(insertChild()));
            menu.addAction(newActs);
    }
    else if(index.data(Qt::UserRole+1).toInt()>11){
        QAction *newActa = new QAction(QIcon(":/Resource/warning32.ico"), tr("&Добавить"), this);
        newActa->setStatusTip(tr("new sth"));
        connect(newActa, SIGNAL(triggered()), this, SLOT(insertChild()));
        menu.addAction(newActa);
    }
    menu.exec(ui->treeView->mapToGlobal(pos));
}
