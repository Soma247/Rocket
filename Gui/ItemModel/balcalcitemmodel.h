#ifndef BALCALCITEMMODEL_H
#define BALCALCITEMMODEL_H
#include "ballisticcalc.h"
#include <QAbstractItemModel>

class balcalcItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit balcalcItemModel(std::unique_ptr<ballisticCalculator> bc, QObject *parent = nullptr);
    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;




    void setNosecone(matherial math,double Dend, double len, double delta);
    void addConoid(matherial math, double Dbegin, double Dend, double length, double delta);
    void addplane(matherial math,
                  double Xfromnose,
                  double Broot, double Btip,
                  double Croot, double Ctip,
                  double Xtip, double Xrf,
                  double Xrr, double Xtf,
                  double Xtr, double H);

    void addEquipment(std::string eqname, double X, double mass);

    void openProject(std::string proFile){
       balcal->openProject(proFile);
    }
    void saveProject(std::string proFile)const{
        balcal->saveProject(proFile);
    }
    std::vector<size_t>modelstate()const{
        return balcal->state();
    }

private:
    std::unique_ptr<ballisticCalculator> balcal;
};

#endif // BALCALCITEMMODEL_H
