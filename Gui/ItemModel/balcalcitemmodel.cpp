#include "balcalcitemmodel.h"

balcalcItemModel::balcalcItemModel(std::unique_ptr<ballisticCalculator> bc, QObject *parent)
    : QAbstractItemModel(parent)
{balcal.reset(bc.release());
}

QVariant balcalcItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex balcalcItemModel::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex balcalcItemModel::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int balcalcItemModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int balcalcItemModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant balcalcItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
