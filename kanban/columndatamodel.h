#ifndef COLUMNDATAMODEL_H
#define COLUMNDATAMODEL_H


#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QStringList>
#include <QStringListModel>

class ColumnDataModel : public QAbstractItemModel
{
   Q_OBJECT

    QStringList taskList;

public:
    ColumnDataModel(QObject *parent);

    // Model interface implementation

    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &index) const;

public slots:
    void addTask(QString &description, QString &datetimeCreated, QString &deadline);
    void removeTask();
};

#endif // DATAMODEL_H


#endif // COLUMNDATAMODEL_H
