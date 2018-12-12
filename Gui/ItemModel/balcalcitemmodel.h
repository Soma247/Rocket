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

private:
    std::unique_ptr<ballisticCalculator> balcal;
};

#endif // BALCALCITEMMODEL_H
