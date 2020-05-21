#include "columndatamodel.h"

ColumnDataModel::ColumnDataModel(QObject *parent): QStringListModel(parent)
{

}

Qt::ItemFlags ColumnDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractListModel::flags(index) | Qt::ItemIsDropEnabled;
    return QAbstractListModel::flags(index) | Qt::ItemIsDragEnabled;
}
