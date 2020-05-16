#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H


#include "singleton.h"
#include "schemacreatequery.h"
#include "sqltablemodelextension.h"
#include "taskmanager.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>
#include <QSqlField>
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QPair>
#include <tuple>

using ColumnKey = QPair<QString, QString>; // board_name, name
using TaskKey = std::tuple<QString, QString, QString>; // board_name, column_name, datetime_created

class DatabaseManager: public Singleton<DatabaseManager>
{

public:
    DatabaseManager();

    bool insertBoard(QString& boardName, QString* description = nullptr, QString* pathToBackground = nullptr);
    bool updateBoard(QString& boardName, QString* newBoardName, QString* newDescription = nullptr, QString* newPathToBackground = nullptr);
    bool deleteBoard(QString& boardName);
    void selectBoards(QSqlTableModel& model);
    QSqlRecord selectBoard(QString& boardName);

    bool insertBackColumn(ColumnKey& columnKey);
    bool updateColumnPos(ColumnKey& columnKey, quint8& prevPos, quint8& newPos);
    bool updateColumnName(ColumnKey& columnKey, QString& newColumnName);
    bool deleteColumn(ColumnKey& columnKey, quint8 &prevPos);
    void selectColumnsByBoardName(QSqlTableModel& model, QString& boardName);
    QSqlRecord selectColumn(ColumnKey& columnKey);
    quint8 findMaxColumnPosInBoard(QString boardName);

    bool insertTask(TaskKey& taskKey, QString& description, quint8& pos, QString* deadline = nullptr);
    bool moveTaskToOtherColumn(TaskKey& taskKey, QString& newColumnName, quint8& prevPos, quint8& newPos);
    bool updateTaskPosInColumn(TaskKey& taskKey, quint8& prevPos, quint8& newPos);
    bool updateTaskDescription(TaskKey& taskKey, QString& newDescription);
    bool deleteTask(TaskKey& taskKey, quint8 &prevPos);
    QSqlRecord selectTask(TaskKey& taskKey);
    void selectTasksByBoardNameAndColumnName(QSqlTableModel& model, QString& boardName, QString& columnName);

private:
    void bindColumnKey(QSqlQuery& query, ColumnKey& columnKey);
    void bindTaskKey(QSqlQuery& query, TaskKey& taskKey);
    void selectTasks(QSqlTableModel& model, TaskKey &taskKey, quint8 begin, quint8 end = 255);
    void selectColumns(QSqlTableModel& model, ColumnKey& columnKey, quint8 begin, quint8 end = 255);
    void shrinkModel(QSqlTableModel& model, quint8 begin, quint8 end = 255);

private:
    QSqlDatabase database;
};

#endif // DATABASEMANAGER_H
