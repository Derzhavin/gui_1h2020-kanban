#ifndef COLUMN_H
#define COLUMN_H

#include "config.h"
#include "projectwindow.h"
#include "customtasklistview.h"
#include "columndatamodel.h"

#include <QListView>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QObject>
#include <QStringListModel>

class ColumnWidget: public QWidget
{
    Q_OBJECT

public:
    ColumnWidget(QString columnName, QWidget *parent = nullptr);

    void setColumnName(QString name);
    void removeTask(QModelIndex& index);
    void pushFrontTask(QString& description, QString& datetimeCreated, QString& deadline);
    void updateTaskAt(QModelIndex &index, QString &description, QString &deadline);
    QString getTaskCreatedAt(quint8 pos);

    QString getColumnWidgetName();

public slots:
    void taskChosen(QModelIndex index, QPoint clickPos);

signals:
    void taskChosen(ColumnWidget* columnWidget, QModelIndex& index, QPoint& clickPos);

public:
    QPushButton *addTaskPushButton;
    QPushButton *removeColumnPushButton;
    QPushButton *renameColumnPushButton;
    QLabel *columnNameLabel;
    CustomTaskListView *tasksListView;

    ColumnDataModel *columnDataModel;
    QStringList datetimeCreatedList;
    QString columnName;
};

#endif // COLUMN_H
