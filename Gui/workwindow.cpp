#include <QStandardItem>
#include <QPair>
#include <QVariant>
#include <fstream>
#include <QMessageBox>
#include <sstream>
#include <QFileDialog>
#include <QStringList>
#include <QThread>
#include "workwindow.h"
#include "ui_workwindow.h"
#include "Dialogs/errordialog.h"

#include <iostream>
Q_DECLARE_METATYPE(OutputData)
constexpr int MAX_lastprojects=10;

WorkWindow::WorkWindow(QString materialsfile, QString hfuelsfile, QString lastprojectsfile, QWidget *parent) :
    QMainWindow(parent),ui(new Ui::WorkWindow),profile{lastprojectsfile.toStdString()},matfile{materialsfile.toStdString()},
    flfile{hfuelsfile.toStdString()}
{
    qRegisterMetaType<OutputData>("OutputData");
    ui->setupUi(this);
    QThread* bcThread=new QThread(this);
    //register the model
    auto bc=new balcalcItemModel(std::make_unique<ballisticCalculator>(new fInterpAtmosphere("1.txt")));
    bc->moveToThread(bcThread);
    bcThread->start();

    ui->treeView->setModel(bc);
    updateActions();
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeView->setAutoExpandDelay(0);
    connect(bc,SIGNAL(updated()),ui->treeView,SLOT(expandAll()));
    connect(bc,SIGNAL(updated()),this,SLOT(updateActions()));
    connect(bc,SIGNAL(calculateEnd(OutputData)),this,SLOT(showresult(OutputData)));
    connect (this,SIGNAL(startCalculate()),bc,SLOT(calculate()));
    try{
        std::ifstream in(lastprojectsfile.toStdString());
        std::string temp;
        QString tempq;
        QStringList lst;
        while(in){
            in>>temp;
            bool dup=false;
            for(const auto& fn:fnames)
                if(fn.second.toStdString()==temp){dup=true;break;}

            if(!dup){
                lst=QString::fromStdString(temp).split("/");
                if(!temp.size()||!lst.size())break;
                fnames.push_back(std::pair<QString,QString>(lst.back(),QString::fromStdString(temp)));
            }
        }

        while(fnames.size()>MAX_lastprojects)
            fnames.pop_back();

        addconedial=new addConoidDialog(&materials,bc,this);
        addconedial->setModal(true);
        ncdial=new SetNoseConeDialog(&materials,bc,this);
        ncdial->setModal(true);
        addplane=new addPlaneDialog(&materials,bc,this);
        addplane->setModal(true);
        addeqdial=new addequipmentDialog(bc,this);
        addeqdial->setModal(true);
        setflytaskdial=new SetFlyTask(&materials,&hardfuels,bc,this);
        setflytaskdial->setModal(true);
        choosedial=new chooseWindow(&fnames,this);
        aboutdial=new about(this);
        aboutdial->setModal(true);
        errd=new errorDialog("some err",true,this);
        errd->setModal(true);
        connect(choosedial, SIGNAL(openfile(std::string)), this, SLOT(openFile(std::string)));
        connect(choosedial, SIGNAL(openfileD()), this, SLOT(openFile()));
        connect(choosedial, SIGNAL(newproject()), this, SLOT(newProject()));
        choosedial->show();
        menuBar()->setStyleSheet(ui->textBrowser->styleSheet());
    }
    catch(std::exception& e){
        errd->setdata(e.what(),true);
        errd->show();
    }
    catch(...){
        errd->setdata("sorry",true);
        errd->show();
    }

    if(!readMaterials(materialsfile)){
        errd->setdata(QString("Ошибка: Файл %1 %2").arg(materialsfile).arg(" поврежден."),true);
        errd->show();
    }
    if(!readHardfuels(hfuelsfile)){
        errd->setdata(QString("Ошибка: Файл %1 %2").arg(hfuelsfile).arg(" поврежден."),true);
        errd->show();
    }
    editfuelmatdial=new EditFuels(&materials,&hardfuels,this);
    editfuelmatdial->setModal(true);
    connect(editfuelmatdial,SIGNAL(matupdated()),this,SLOT(savemats()));
    connect(editfuelmatdial,SIGNAL(flupdated()),this,SLOT(savefuels()));
    resultw=new resultWindow(this);
}

WorkWindow::~WorkWindow()
{
    delete ui;
    delete addconedial;
    delete errd;
    delete ncdial;
    delete addplane;
    delete addeqdial;
    delete setflytaskdial;
    delete choosedial;
    delete aboutdial;
    delete errd;
}




void WorkWindow::selectionChangedSlot(const QItemSelection &, const QItemSelection &)
{
}

void WorkWindow::on_treeView_customContextMenuRequested(const QPoint &pos){
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    if(!index.isValid())return;
    curindex=static_cast<size_t>(index.row());
    QMenu menu(this);
    menu.setStyleSheet(ui->textBrowser->styleSheet());
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
        if(isfirstSetInputData){
            isfirstSetInputData=false;
            QAction *newActr = new QAction(tr("&Выбрать"), this);
            newActr->setStatusTip(tr("set"));
            connect(newActr, SIGNAL(triggered()), this, SLOT(setFlyTaskDialog()));
            menu.addAction(newActr);
            break;
        }
        else{
            QAction *newActr = new QAction(tr("&Редактировать"), this);
            newActr->setStatusTip(tr("edit"));
            connect(newActr, SIGNAL(triggered()), this, SLOT(setFlyTaskDialog()));
            menu.addAction(newActr);
            break;
        }
    }
    case int(balcalcItem::itemtype::TailStabCont):{
        QAction *newActs = new QAction(tr("&Установить"), this);
        newActs->setStatusTip(tr("set"));
        connect(newActs, SIGNAL(triggered()), this, SLOT(SetTailStabDialog()));
        menu.addAction(newActs);
        break;
    }
    case int(balcalcItem::itemtype::TailStab):{
        QAction *newActs = new QAction(tr("&Изменить"), this);
        newActs->setStatusTip(tr("set"));
        connect(newActs, SIGNAL(triggered()), this, SLOT(EditTailStabDialog()));
        menu.addAction(newActs);

        QAction *newActs2 = new QAction(tr("&Удалить"), this);
        newActs->setStatusTip(tr("del"));
        connect(newActs2, SIGNAL(triggered()), this, SLOT(RemoveTailStabDialog()));
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
        QAction *newActa = new QAction(tr("&Добавить"), this);
        newActa->setStatusTip(tr("add"));
        connect(newActa, SIGNAL(triggered()), this, SLOT(SetStabDialog()));
        menu.addAction(newActa);
        break;
    }
    case int(balcalcItem::itemtype::Plane):{
        QAction *newActa = new QAction(tr("&Редактировать"), this);
        newActa->setStatusTip(tr("edit"));
        connect(newActa, SIGNAL(triggered()), this, SLOT(EditStabDialog()));
        menu.addAction(newActa);
        QAction *newActa2 = new QAction(tr("&Удалить"), this);
        newActa->setStatusTip(tr("del"));
        connect(newActa2, SIGNAL(triggered()), this, SLOT(RemoveStabDialog()));
        menu.addAction(newActa2);
        break;

    }
    case int(balcalcItem::itemtype::EquipsCont):{
        QAction *newActa = new QAction(tr("&Добавить"), this);
        newActa->setStatusTip(tr("add"));
        connect(newActa, SIGNAL(triggered()), this, SLOT(AddEqDialog()));
        menu.addAction(newActa);
        break;
    }
    case int(balcalcItem::itemtype::Equipment):{
        QAction *newActa = new QAction(tr("&Редактировать"), this);
        newActa->setStatusTip(tr("edit"));
        connect(newActa, SIGNAL(triggered()), this, SLOT(EditEqDialog()));
        menu.addAction(newActa);
        QAction *newActa2 = new QAction(tr("&Удалить"), this);
        newActa->setStatusTip(tr("del"));
        connect(newActa2, SIGNAL(triggered()), this, SLOT(RemoveEqDialog()));
        menu.addAction(newActa2);
        break;
    }

    }
    menu.exec(ui->treeView->mapToGlobal(pos));
}




void WorkWindow::updateActions(const QItemSelection &,const QItemSelection &) {
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
    msgBox.setStyleSheet(ui->textBrowser->styleSheet());
    msgBox.setInformativeText("Вы действительно хотите удалить данный отсек?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    //поправить вид диалога
    int ret = msgBox.exec();
    if(ret==QMessageBox::Ok){
        balcalcItemModel* model=dynamic_cast<balcalcItemModel*>(ui->treeView->model());
        if(model && curindex<model->modelstate().at(2))
            model->ejectConoid(curindex);
    }
}

void WorkWindow::SetTailStabDialog()
{
    addplane->setdata(true,false);
    addplane->show();
}

void WorkWindow::EditTailStabDialog()
{
    addplane->setdata(true,true);
    addplane->show();
}

void WorkWindow::RemoveTailStabDialog()
{
    QMessageBox msgBox(this);
    msgBox.setStyleSheet(ui->textBrowser->styleSheet());
    msgBox.setInformativeText("Вы действительно хотите удалить хвостовой стабилизатор?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    //поправить вид диалога
    int ret = msgBox.exec();
    if(ret==QMessageBox::Ok)
        dynamic_cast<balcalcItemModel*>(ui->treeView->model())->ejectTailStab();

}

void WorkWindow::SetStabDialog()
{
    addplane->setdata(false,false);
    addplane->show();
}

void WorkWindow::EditStabDialog()
{
    addplane->setdata(false,true,int(curindex));
    addplane->show();
}

void WorkWindow::RemoveStabDialog()
{
    QMessageBox msgBox(this);
    msgBox.setStyleSheet(ui->textBrowser->styleSheet());
    msgBox.setInformativeText("Вы действительно хотите удалить данную плоскость?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    //поправить вид диалога
    int ret = msgBox.exec();
    if(ret==QMessageBox::Ok)
        dynamic_cast<balcalcItemModel*>(ui->treeView->model())->ejectPlane(curindex);
}

void WorkWindow::AddEqDialog()
{
    addeqdial->setdata();
    addeqdial->show();

}

void WorkWindow::EditEqDialog()
{
    addeqdial->setdata(int(curindex));
    addeqdial->show();
}

void WorkWindow::RemoveEqDialog()
{
    QMessageBox msgBox(this);
    msgBox.setStyleSheet(ui->textBrowser->styleSheet());
    msgBox.setInformativeText("Вы действительно этот груз?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    //поправить вид диалога
    int ret = msgBox.exec();
    if(ret==QMessageBox::Ok)
        dynamic_cast<balcalcItemModel*>(ui->treeView->model())->ejectEquipment(curindex);
}

void WorkWindow::setFlyTaskDialog()
{
    setflytaskdial->setdata(!isfirstSetInputData);
    if(isfirstSetInputData)isfirstSetInputData=false;
    setflytaskdial->show();
}

void WorkWindow::openFile()
{
    QString str = QFileDialog::getOpenFileName(this, tr("Загрузить проект"), "D:/Qt/projects/Rocket", tr("BC Project Files (*.bcpr)"));
    if(!str.isEmpty())
    openFile(str.toStdString());
}

void WorkWindow::saveFile()
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Сохранить проект", "", "*.bcpr");
    if(!str.isEmpty())
    saveFile(str.toStdString());
}
void WorkWindow::saveFile(std::string filename){
 //   errd->setdata(QString::fromStdString(filename),false);
 //   errd->show();
    balcalcItemModel* bc=dynamic_cast<balcalcItemModel*>(ui->treeView->model());
    if(bc){
        try{
            bc->saveProject(filename);
            fnames.push_front(std::pair<QString,QString>(
                                  QString::fromStdString(filename).split("/").back(),
                                  QString::fromStdString(filename)));
            savefnames();
        }
        catch(ballisticCalculator::error_reading_file&){
            errd->setdata(QString("Файл %1 недоступен").arg(QString::fromStdString(filename)),false);
            errd->show();
        }
        catch(...){
            errd->setdata(QString("неизвестная ошибка при сохранении проекта в файл \n%1").
                                             arg(QString::fromStdString(filename)),false);
            errd->show();
        }
    }
    else{
        errd->setdata(QString("bcmod_error"),true);
        errd->show();
    }
}

void WorkWindow::newProject(){
    balcalcItemModel* bc=dynamic_cast<balcalcItemModel*>(ui->treeView->model());
    if(bc){
        try{
            bc->clear();
            choosedial->hide();
            this->show();
        }
        catch(ballisticCalculator::error_reading_file&){
            errd->setdata(QString("Создание файла невозможно"),false);
            errd->show();
        }
        catch(...){
            errd->setdata(QString("неизвестная ошибка"),false);
            errd->show();
        }
    }
}

void WorkWindow::savemats()
{
    std::ofstream out(this->matfile);
    if(!out){
        errd->setdata(QString("файл материалов недоступен"),false);
        errd->show();
        return;
    }

    for(auto&m:materials)
        out<<m<<std::endl;
}

void WorkWindow::savefuels()
{
    std::ofstream out(this->flfile);
    if(!out){
        errd->setdata(QString("файл топлив недоступен"),false);
        errd->show();
        return;
    }
    for(auto&f:hardfuels)
        out<<f<<std::endl;
}

void WorkWindow::openFile(std::string filename)
{
    balcalcItemModel* bc=dynamic_cast<balcalcItemModel*>(ui->treeView->model());
    if(bc)
        try{
        bc->openProject(filename);
        if(filename.size())
            fnames.push_front(std::pair<QString,QString>(
                                  QString::fromStdString(filename).split("/").back(),
                                  QString::fromStdString(filename)));
        savefnames();
        choosedial->hide();
        this->show();
    }
    catch(ballisticCalculator::error_reading_file&){
        errd->setdata(QString("Файл %1 поврежден").arg(QString::fromStdString(filename)),false);
        errd->show();
    }
    catch(...){
        errd->setdata(QString("неизвестная ошибка").arg(QString::fromStdString(filename)),false);
        errd->show();
    }
}

void WorkWindow::savefnames()
{
//    std::cerr<<"savefnames:"<<fnames.size()<<std::endl;
    try {
        while(fnames.size()>MAX_lastprojects)
            fnames.pop_back();
        std::ofstream out (profile,std::ios_base::out);
        if(!out)throw std::exception();
        for(auto&fn:fnames)
            out<<fn.second.toStdString()<<std::endl;

    } catch (...) {
        errd->setdata(QString("файл недоступен: ").arg(QString::fromStdString(profile)),false);
        errd->show();
    }
}



void WorkWindow::updateActions() {
    balcalcItemModel* bc=dynamic_cast<balcalcItemModel*>(ui->treeView->model());
    if(!bc){
        errd->setdata(QString("критическая ошибка баллистического калькулятора").arg(QString::fromStdString(profile)),true);
        errd->show();
    }
    InputData id=bc->getInputData();
    RocketHeadData rh=bc->getheaddata();
    QString mes=QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li {white-space: pre-wrap; }</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600;\">Параметры ракеты:</span></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Длина головной части: %1 м;</span></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Масса головной части: %2 кг;</span></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Диаметр РДТТ: %3 м;</span></p><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Максимальный диаметр: %4 м;</span></p>").arg(rh.headLen).arg(rh.headMass).arg(rh.headDend).arg(rh.headDmax);
    mes+="<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\"> </span></p>";
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Отсеки:</span><span style=\" font-size:7pt;\"> D0(м)    D1(м)    L(м)    масса(кг):</span></p>");
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Отсек 1: %1  %2  %3  %4;</span></p>").arg(rh.nconepar.dbeg,0,'D',2).arg(rh.nconepar.dend,0,'D',2).arg(rh.nconepar.len,0,'D',2).arg(rh.nconepar.mass,0,'D',1);
    for(size_t i=0;i<rh.conespar.size();i++){
        mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Отсек %1: %2  %3  %4  %5;</span></p>").arg(i+2).arg(rh.conespar.at(i).dbeg,0,'D',2).arg(rh.conespar.at(i).dend,0,'D',2).arg(rh.conespar.at(i).len,0,'D',2).arg(rh.conespar.at(i).mass,0,'D',1);
    }
    mes+="<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\"> </span></p>";

    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Плоскости(1 конс):</span><span style=\" font-size:7pt;\"> S(кв.м)  X(м)  масса (кг):</span></p>");
    size_t i=0;
    for(;i<rh.planespar.size();i++){
        mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Плоскость %1: %2  %3  %4;</span></p>").arg(i+1).arg(rh.planespar.at(i).S(),0,'D',2).arg(rh.planespar.at(i).XfromNose,0,'D',2).arg(rh.planespar.at(i).massn/4,0,'D',2);
    }
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Хвост. стаб : %1           %2;</span></p>").arg(rh.tailstabpar.S(),0,'D',2).arg(rh.tailstabpar.massn/4,0,'D',2);
    mes+="<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\"> </span></p>";
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Грузы:</span><span style=\" font-size:7pt;\">Координата(м)  масса (кг):</span></p>");
    for(auto&e:rh.equipspar)
        mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">%1: %2  %3;</span></p>").arg(QString::fromStdString(e.name),8).arg(e.X,0,'D',2).arg(e.m,0,'D',2);
    mes+="<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\"> </span></p>";
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Исходные данные:</span></p>");
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Макс. высота полета цели(м): %1;</span></p>").arg(id.Hmax);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Мин. высота полета ракеты(м): %1;</span></p>").arg(id.Hmin);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Дальняя граница зоны поражения(м): %1;</span></p>").arg(id.Xmax);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Рубеж выполнения задания(м): %1;</span></p>").arg(id.mstone);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Скорость цели(м/с): %1;</span></p>").arg(id.Vtar);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Давление на срезе сопла(мПА): %1;</span></p>").arg(id.enPa);
    mes+=QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Давление в кр. сечении сопла(мПА): %1;</span></p>").arg(id.enPk);
    mes+="</body></html>";
    ui->textBrowser->setHtml(mes);
    if(id.iscorrect()&&rh.iscorrect())
        ui->btn_balcalculate->setDisabled(false);
    else
        ui->btn_balcalculate->setDisabled(true);
}

bool WorkWindow::readMaterials(QString matfile){
    std::ifstream in(matfile.toStdString());
    if(!in)return false;
    material mat;
    while(in>>mat){
        materials.push_back(mat);
    }
    if(in.eof())return true;
    return false;
}

bool WorkWindow::readHardfuels(QString hfuelsfile){
    std::ifstream in(hfuelsfile.toStdString());
    if(!in)return false;
    fuel fl;
    while(in>>fl)
        if(in)hardfuels.push_back(fl);
    if(in.eof())return true;
    return false;
}

void WorkWindow::on_action_open_triggered(){
    openFile();
}

void WorkWindow::on_action_save_as_triggered(){
    saveFile();
}

void WorkWindow::on_action_save_triggered(){
    saveFile(fnames.begin()->second.toStdString());
}

void WorkWindow::on_action_new_triggered()
{
    newProject();
}

void WorkWindow::on_about_triggered()
{
    aboutdial->show();
}

void WorkWindow::on_btn_balcalculate_clicked()
{
     balcalcItemModel* bc=dynamic_cast<balcalcItemModel*>(ui->treeView->model());
     if(bc){
         OutputData odat;
         emit startCalculate();
         ui->btn_balcalculate->setDisabled(true);
         ui->treeView->setDisabled(true);
         ui->menu->setDisabled(true);
     }
     else{
         errd->setdata(QString("Критическая ошибка 3."),false);
         errd->show();
     }
}

void WorkWindow::on_flmatedit_triggered()
{
    editfuelmatdial->show();
}

void WorkWindow::showresult(OutputData odat)
{
    if(odat.iscorrect()){
        resultw->setdata(odat,fnames.front().second);
        resultw->show();
        ui->btn_balcalculate->setDisabled(false);
        ui->treeView->setDisabled(false);
        ui->menu->setDisabled(false);
        this->hide();
    }
    else{
        errd->setdata(QString("точка встречи не достигнута с заданной скоростью."),false);
        errd->show();
    }
}
