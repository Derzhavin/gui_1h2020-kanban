#include "columndatamodel.h"

ColumnDataModel::ColumnDataModel(QString columnName, QObject *parent) : QAbstractItemModel(parent)
{
    this->columnName = columnName;
}

int ColumnDataModel::columnCount(const QModelIndex &) const
{
    return 1;
}

int ColumnDataModel::rowCount(const QModelIndex &) const
{
    return taskList.size();
}

QVariant ColumnDataModel::data(const QModelIndex &index, int role) const
{
    if( role == Qt::DisplayRole )
    {
        return taskList.at(index.row());
    }
    return QVariant();
}

QModelIndex ColumnDataModel::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column, (void*)&taskList[row]);
}

QModelIndex ColumnDataModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

void ColumnDataModel::addTask(QString &description, QString &datetimeCreated, QString &deadline)
{
    QString value = QString(description + "\n" +
                           "created at: " + datetimeCreated +
                           (deadline.isEmpty() ? "": "\ndeadline: " + deadline));
    taskList.append(value);
    datetimeCreatedList.append(datetimeCreated);

    emit layoutChanged();
}

void ColumnDataModel::removeTask(QString &datetimeCreated)
{
    if (!taskList.empty()) {
        quint8 pos = datetimeCreatedList.indexOf(datetimeCreated);
        taskList.removeAt(pos);
        datetimeCreatedList.removeAt(pos);
        emit layoutChanged();
    }
}
