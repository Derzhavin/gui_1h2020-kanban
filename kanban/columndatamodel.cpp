#include "columndatamodel.h"

ColumnDataModel::ColumnDataModel(QObject *parent) : QAbstractItemModel(parent)
{

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
    emit layoutChanged();
}

void ColumnDataModel::removeTask()
{
    if (!taskList.empty()) {
        taskList.removeLast();
        emit layoutChanged();
    }
}
