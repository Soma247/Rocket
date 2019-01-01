#include "balcalcitemmodel.h"
constexpr size_t statevlen=6;
//конструктор
balcalcItemModel::balcalcItemModel(std::unique_ptr<ballisticCalculator> bc, QObject *parent)
    : QAbstractItemModel(parent){
    balcal.reset(bc.release());

    rootItem=new balcalcItem(balcalcItem::itemtype::Project,"Проект");
    balcalcItem *rocket =new balcalcItem(balcalcItem::itemtype::RocketModel,"Модель ракеты");
        nosecone =new balcalcItem(balcalcItem::itemtype::NoseconeCont,"Носовой обтекатель");
        tailstab =new balcalcItem(balcalcItem::itemtype::TailStabCont,"Хвостовой стабилизатор");
        stabs =new balcalcItem(balcalcItem::itemtype::PlanesCont,"Несущие плоскости");
        cones =new balcalcItem(balcalcItem::itemtype::ConesCont,"Отсеки");
        equips =new balcalcItem(balcalcItem::itemtype::EquipsCont,"Аппаратура(грузы)");
    flytask =new balcalcItem(balcalcItem::itemtype::FlyTask,"Исходные данные");
    rootItem->appendChild(rocket);
    rootItem->appendChild(flytask);

    rocket->appendChild(nosecone);
    rocket->appendChild(tailstab);
    rocket->appendChild(stabs);
    rocket->appendChild(cones);
    rocket->appendChild(equips);

}

balcalcItemModel::~balcalcItemModel(){
    //delete rootItem;
}


int balcalcItemModel::columnCount(const QModelIndex &) const {return rootItem->columnCount();}

QVariant balcalcItemModel::data (const QModelIndex &index, int role) const {
 if (!index.isValid()) return QVariant();
 if (role != Qt::DisplayRole  && role != Qt::EditRole && role!=Qt::UserRole+1) return QVariant();
 balcalcItem *item = getItem(index);
 return item->data(index.column(),role);
}

balcalcItem *balcalcItemModel::getItem(const QModelIndex &index) const {
 if (index.isValid()) {
  balcalcItem *item = static_cast<balcalcItem*>(index.internalPointer());
  if (item) return item;
 }
 return rootItem;
}

void balcalcItemModel::clear()
{
    if(balcal){
        balcal->clear(true);
    }
    update();
}

void balcalcItemModel::setInputData(const InputData &indat){
    if(balcal)balcal->setInputData(indat);
    update();
}

InputData balcalcItemModel::getInputData(){return balcal?balcal->getInputData():InputData();}

void balcalcItemModel::setNosecone(material math, double Dend, double len, double delta)
{
    if(balcal)
        balcal->setNosecone(math,Dend,len,delta);
    update();
}

void balcalcItemModel::setTailStab(material math, double Broot, double Btip, double Croot, double Ctip, double Xtip, double Xrf, double Xrr, double Xtf, double Xtr, double H){
    if(balcal)
        balcal->setTailStab(math,0,Broot,Btip,Croot,Ctip,Xtip,Xrf,Xrr,Xtf,Xtr,H);
    update();
}

void balcalcItemModel::addConoid(material math, double Dbegin, double Dend, double length, double delta)
{
   if(balcal){
    balcal->addConoid(math,Dbegin,Dend,length,delta);
   }
    update();
}

void balcalcItemModel::addplane(material math, double Xfromnose, double Broot, double Btip, double Croot, double Ctip, double Xtip, double Xrf, double Xrr, double Xtf, double Xtr, double H)
{
    if(balcal)
        balcal->addplane(math,Xfromnose,Broot,Btip,Croot,Ctip,Xtip,Xrf,Xrr,Xtf,Xtr,H);
    update();
}

void balcalcItemModel::calculate(){
    if(!balcal)throw std::exception();
    emit calculateEnd(balcal->calculate());
}

void balcalcItemModel::addEquipment(std::string eqname, double X, double mass){
    if(balcal)
        balcal->addEquipment(eqname,X,mass);
    update();
}

void balcalcItemModel::ejectConoid(size_t index){
    if(balcal)
        balcal->ejectConoid(index);
     update();
}

void balcalcItemModel::ejectPlane(size_t index){
    if(balcal)
        balcal->ejectPlane(index);
     update();
}

void balcalcItemModel::ejectEquipment(size_t index){
    if(balcal)
        balcal->ejectEquipment(index);
     update();
}

void balcalcItemModel::ejectTailStab(){
    if(!balcal)return;
    balcal->ejectTailStab();
    update();
}


void balcalcItemModel::insertConoid(material math, double Dbegin, double Dend, double length, double delta, size_t index){
    if(balcal)
        balcal->insertConoid(math,Dbegin,Dend,length,delta,index);
    update();
}

void balcalcItemModel::openProject(std::string proFile){
    balcal->openProject(proFile);
    update();
}

void balcalcItemModel::saveProject(std::string proFile) const{
    balcal->saveProject(proFile);
}

std::vector<size_t> balcalcItemModel::modelstate() const{
    return balcal->state();
}

void balcalcItemModel::update(){
    beginResetModel();
    if(rootItem&&balcal){
        std::vector<size_t> state=balcal->state();
        nosecone->removeAllChildrens();
        tailstab->removeAllChildrens();
        stabs->removeAllChildrens();
        cones->removeAllChildrens();
        equips->removeAllChildrens();

        if(state.size()==statevlen){
           headData=balcal->getheadData();
            if(state.at(0))
                nosecone->appendChild(
                            new balcalcItem(balcalcItem::itemtype::Nosecone,"Носовой обтекатель",nosecone));
            if(state.at(1))
                tailstab->appendChild(
                            new balcalcItem(balcalcItem::itemtype::TailStab,"Хвостовое оперение",tailstab));
            for(size_t i=0;i<state.at(2);++i)
                cones->appendChild(
                            new balcalcItem(balcalcItem::itemtype::Cone,QString("Отсек %1").arg(i+2),cones));

            for(size_t i=0;i<state.at(3);++i)
                stabs->appendChild(
                            new balcalcItem(
                                balcalcItem::itemtype::Plane,QString("Несущая плоскость %1").arg(i+1),stabs));

            for(size_t i=0;i<state.at(4);++i)
                equips->appendChild(
                            new balcalcItem(balcalcItem::itemtype::Equipment,
                                            QString::fromStdString(headData.equipspar.at(i).name),
                                            equips));
        }
    }
    endResetModel();
    emit updated();
}

const RocketHeadData &balcalcItemModel::getheaddata() const{
    return headData;
}

Qt::ItemFlags balcalcItemModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return nullptr;
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

QVariant balcalcItemModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) return rootItem->data(section,role);
    return QVariant();
}

QModelIndex balcalcItemModel::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() && parent.column() != 0) return QModelIndex();
    balcalcItem *parentItem = getItem(parent);
    balcalcItem *childItem = parentItem->child(row);
    if (childItem) return createIndex(row, column, childItem);
    else return QModelIndex();
}

QModelIndex balcalcItemModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) return QModelIndex();
    balcalcItem *childItem = getItem(index);
    balcalcItem *parentItem = childItem->parentItem();
    if (parentItem == rootItem) return QModelIndex();
    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int balcalcItemModel::rowCount(const QModelIndex &parent) const {
    balcalcItem *parentItem = getItem(parent);
    return parentItem->childCount();
}

