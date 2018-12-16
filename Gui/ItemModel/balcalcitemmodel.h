#ifndef BALCALCITEMMODEL_H
#define BALCALCITEMMODEL_H
#include "ballisticcalc.h"
#include "balcalcitem.h"
#include <QAbstractItemModel>

class balcalcItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit balcalcItemModel(std::unique_ptr<ballisticCalculator> bc, QObject *parent = nullptr);
    ~balcalcItemModel()override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int position, int,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;


    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override{
        std::cout<<"setdata"<<std::endl;
        return true;}
      //установить данные узла с индексом index в значение value
     bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                        int role = Qt::EditRole) override{
          std::cout<<"sethdata"<<std::endl;
          return true;
     }
      //установить данные заголовка столбца
     bool insertColumns(int position, int columns,const QModelIndex &parent = QModelIndex()) override{
          std::cout<<"insertcol"<<std::endl;
          return true;}
     bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex()) override{
          std::cout<<"remcol"<<std::endl;return true;}
     bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override{
          std::cout<<"insrow"<<std::endl;return true;}
     bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override{
          std::cout<<"remrow"<<std::endl;return true;}
      //вставка и удаление столбцов и строк

    balcalcItem* getItem(const QModelIndex &index) const;


    void setNosecone(matherial math,double Dend, double len, double delta);
    void setTailStab(matherial math,
                     double Broot, double Btip,
                     double Croot, double Ctip,
                     double Xtip, double Xrf,
                     double Xrr, double Xtf,
                     double Xtr, double H);
    void addConoid(matherial math, double Dbegin, double Dend, double length, double delta);
    void addplane(matherial math,
                  double Xfromnose,
                  double Broot, double Btip,
                  double Croot, double Ctip,
                  double Xtip, double Xrf,
                  double Xrr, double Xtf,
                  double Xtr, double H);

    void addEquipment(std::string eqname, double X, double mass);


    void ejectConoid(size_t index);
    void ejectPlane(size_t index);
    void ejectEquipment(size_t index);
    void insertConoid(matherial math, double Dbegin, double Dend, double length, double delta, size_t index);


    void openProject(std::string proFile){
       balcal->openProject(proFile);
    }
    void saveProject(std::string proFile)const{
        balcal->saveProject(proFile);
    }
    std::vector<size_t>modelstate()const{
        return balcal->state();
    }
    void update();

signals:
    void updated();
private:

    std::unique_ptr<ballisticCalculator> balcal;
    balcalcItem* rootItem;
    balcalcItem* nosecone;
    balcalcItem* tailstab;
    balcalcItem* stabs;
    balcalcItem* cones;
    balcalcItem* equips;
    balcalcItem* flytask;
};

#endif // BALCALCITEMMODEL_H
