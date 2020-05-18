#ifndef COLUMNDATAMODEL_H
#define COLUMNDATAMODEL_H

#include <QStringListModel>

class ColumnDataModel: public QStringListModel
{
    Q_OBJECT
public:
    ColumnDataModel(QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // COLUMNDATAMODEL_H
