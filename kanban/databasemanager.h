#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H


#include "singleton.h"
#include "schemacreatequery.h"
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
    bool updateColumnPos(ColumnKey& columnKey, quint8& newPos);
    bool updateColumnName(ColumnKey& columnKey, QString& newColumnName);
    bool deleteColumn(ColumnKey& columnKey);
    void selectColumnsByBoardName(QSqlTableModel& model, QString& boardName);
    QSqlRecord selectColumn(ColumnKey& columnKey);
    quint8 findMaxColumnPosInBoard(QString boardName);

    bool insertBackTask(TaskKey& taskKey, QString& description, QString* deadline = nullptr);
    bool moveTaskToOtherColumn(TaskKey& taskKey, QString& newColumnName, quint8& newPos); // !
    bool updateTaskPosInColumn(TaskKey& taskKey, quint8& newPos);
    bool updateTaskDescription(TaskKey& taskKey, QString& newDescription);
    bool deleteTask(TaskKey& taskKey);
    QSqlRecord selectTask(TaskKey& taskKey);
    quint8 findMaxTaskPosInColumn(ColumnKey& columnKey);
    void selectTasksByColumn(QSqlTableModel &model, ColumnKey &columnKey);

    static void foreachRecordInModel(QSqlTableModel& model, std::function<void(QSqlRecord& record)> callback);
    static void selectFromTable(QSqlTableModel& model,QString tableName, std::function<void()> callback);

private:
    static void moveRowsByOne(QSqlTableModel& model, bool direction); // true - увеличить, false - уменьшить
    static void bindColumnKey(QSqlQuery& query, ColumnKey& columnKey);
    static void bindTaskKey(QSqlQuery& query, TaskKey& taskKey);

    QSqlDatabase database;
};

#endif // DATABASEMANAGER_H
