#ifndef TASKMANAGER_H
#define TASKMANAGER_H


#include "singleton.h"
#include "schemacreatequery.h"
#include "sqltablemodelextension.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QPair>
#include <tuple>

using ColumnKey = QPair<QString, QString>; // board_name, name
using TaskKey = std::tuple<QString, QString, QString>; // board_name, column_name, datetime_created

class TaskManager: public Singleton<TaskManager>
{

public:
    TaskManager();

    bool addBoard(QString& boardName, QString* description = nullptr, QString* pathToBackground = nullptr);
    bool updateBoard(QString& boardName, QString* newBoardName, QString* newDescription = nullptr, QString* newPathToBackground = nullptr);
    bool removeBoard(QString& boardName);

    bool addColumn(ColumnKey& columnKey, quint8& pos);
    bool updateColumnPos(ColumnKey& columnKey, quint8& prevPos, quint8& newPos);
    bool updateColumnName(ColumnKey& columnKey, QString& newColumnName);
    bool removeColumn(ColumnKey& columnKey, quint8 &prevPos);

    bool addTask(TaskKey& taskKey, QString& description, quint8& pos, QString* deadline = nullptr);
    bool moveTaskToOtherColumn(TaskKey& taskKey, QString& columnName, quint8& prevPos, quint8& newPos);
    bool updateTaskPosInColumn(TaskKey& taskKey, quint8& prevPos, quint8& newPos);
    bool updateTaskDescription(TaskKey& taskKey, QString& newDescription);
    bool removeTask(TaskKey& taskKey, quint8 &prevPos);
    QSqlRecord getTask(TaskKey& taskKey);

private:
    void bindColumnKey(QSqlQuery& query, ColumnKey& columnKey);
    void bindTaskKey(QSqlQuery& query, TaskKey& taskKey);
    void selectTalsks(QSqlTableModel& model, TaskKey &taskKey, quint8 begin, quint8 end = 255);
    void selectColumns(QSqlTableModel& model, ColumnKey& columnKey, quint8 begin, quint8 end = 255);
    void shrinkModel(QSqlTableModel& model, quint8 begin, quint8 end = 255);

private:
    QSqlDatabase database;
};

#endif // TASKMANAGER_H
