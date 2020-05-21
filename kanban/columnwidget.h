#ifndef COLUMN_H
#define COLUMN_H

#include "config.h"
#include "projectwindow.h"
#include "customtasklistview.h"
#include "columndatamodel.h"
#include "taskmanager.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QObject>
#include <QStringListModel>

class ColumnWidget: public QWidget
{
    Q_OBJECT

public:
    ColumnWidget(QString columnName, QWidget *parent = nullptr);
    ColumnWidget(QString columnName, Tasks tasks, QWidget *parent = nullptr);

    void setColumnName(QString name);
    void removeTask(QModelIndex& index);
    void pushFrontTask(QString& description, QString& datetimeCreated, QString& deadline);
    void updateTaskAt(QModelIndex &index, QString &description, QString &deadline);

    QString getTaskDatetimeCreatedAt(TaskUIntT pos);
    void removeTaskDatetimeCreatedAt(TaskUIntT pos);
    void updatePosTaskDatetimeCreatedAt(TaskUIntT posFrom, TaskUIntT posTo);
    void addTaskDatetimeCreatedAt(TaskUIntT pos, QString datetimeCreated);

    QString getColumnWidgetName();

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
