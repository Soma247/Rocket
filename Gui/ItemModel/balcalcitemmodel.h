#ifndef BALCALCITEMMODEL_H
#define BALCALCITEMMODEL_H
#include "ballisticcalc.h"
#include "balcalcitem.h"
#include <QAbstractItemModel>

class balcalcItemModel : public QAbstractItemModel{
    Q_OBJECT
    std::unique_ptr<ballisticCalculator> balcal;
    balcalcItem* rootItem;
    balcalcItem* nosecone;
    balcalcItem* tailstab;
    balcalcItem* stabs;
    balcalcItem* cones;
    balcalcItem* equips;
    balcalcItem* flytask;
    RocketHeadData headData;

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
    int columnCount(const QModelIndex &) const override;


    bool setData(const QModelIndex &, const QVariant &, int) override{return true;}
      //установить данные узла с индексом index в значение value
     bool setHeaderData(int, Qt::Orientation, const QVariant &,int) override{return true;}
      //установить данные заголовка столбца
     bool insertColumns(int, int,const QModelIndex &) override{return true;}
     bool removeColumns(int, int, const QModelIndex &) override{return true;}
     bool insertRows(int, int, const QModelIndex &) override{return true;}
     bool removeRows(int, int, const QModelIndex &) override{return true;}
      //вставка и удаление столбцов и строк

    balcalcItem* getItem(const QModelIndex &index) const;
    void clear();
    void setInputData(const InputData& indat);
    InputData getInputData();
    void setNosecone(material math,double Dend, double len, double delta);
    void setTailStab(material math,
                     double Broot, double Btip,
                     double Croot, double Ctip,
                     double Xtip, double Xrf,
                     double Xrr, double Xtf,
                     double Xtr, double H);
    void addConoid(material math, double Dbegin, double Dend, double length, double delta);
    void addplane(material math,
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
    void ejectTailStab();
    void insertConoid(material math, double Dbegin, double Dend, double length, double delta, size_t index);

    void openProject(std::string proFile);
    void saveProject(std::string proFile)const;
    std::vector<size_t>modelstate()const;
    void update();
    const RocketHeadData& getheaddata()const;
public slots:
       void calculate();

signals:
    void updated();
    void calculateEnd(OutputData odat);
};

#endif // BALCALCITEMMODEL_H
