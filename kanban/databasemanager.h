#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "config.h"
#include "singleton.h"
#include "schemacreatequery.h"

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
    enum class OpStatus {
        Success,
        Failure,
        TableFull,
        NoBoard,
        NoColumn,
        NoTask,
        LongBoardName,
        LongColumnName,
        LongTaskDescription,
        LongBoardDescription
    };

    DatabaseManager();

    OpStatus insertBoard(QString& boardName, QString* description = nullptr, QString* pathToBackground = nullptr);
    OpStatus updateBoard(QString& boardName, QString* newBoardName = nullptr, QString* newDescription = nullptr, QString* newPathToBackground = nullptr);
    OpStatus deleteBoard(QString& boardName);
    void selectBoards(QSqlTableModel& model);
    QSqlRecord selectBoard(QString& boardName);

    OpStatus insertBackColumn(ColumnKey& columnKey);
    OpStatus updateColumnPosInBoard(ColumnKey& columnKey, ColumnUIntT& newPos);
    OpStatus updateColumnName(ColumnKey& columnKey, QString& newColumnName);
    OpStatus deleteColumn(ColumnKey& columnKey);
    void selectColumnsByBoardName(QSqlTableModel& model, QString& boardName);
    QSqlRecord selectColumn(ColumnKey& columnKey);
    ColumnUIntT findMaxColumnPosInBoard(QString boardName);

    OpStatus insertBackTask(TaskKey& taskKey, QString& description, QString* deadline = nullptr);
    OpStatus deleteTask(TaskKey& taskKey);
    OpStatus updateTaskPosInColumn(TaskKey& taskKey, TaskUIntT& newPos);
    OpStatus moveTaskToOtherColumn(TaskKey& taskKey, QString& newColumnName, TaskUIntT newPos = 0); // Если 0, то добавить в конец.
    OpStatus updateTask(TaskKey& taskKey, QString& newDescription, QString *deadline = nullptr);
    QSqlRecord selectTask(TaskKey& taskKey);
    TaskUIntT findMaxTaskPosInColumn(ColumnKey& columnKey);
    void selectTasksByColumn(QSqlTableModel &model, ColumnKey &columnKey);

    void foreachRecordInModel(QSqlTableModel& model, std::function<void(QSqlRecord& record)> callback);
    void selectFromTable(QSqlTableModel& model,QString tableName, std::function<void()> callback);

    QSqlDatabase database;
private:
    OpStatus insertColumn(ColumnKey& columnKey, ColumnUIntT pos);
    OpStatus insertTask(TaskKey &taskKey, TaskUIntT pos, QString& description, QString* deadline = nullptr);
    bool updateColumnPos(ColumnKey& columnKey, ColumnUIntT& newPos);
    bool updateTaskPos(TaskKey& taskKey, TaskUIntT pos);
    OpStatus doTransaction(std::function<OpStatus()> callback);

    static void moveRowsByOne(QSqlTableModel& model, bool direction); // true - увеличить, false - уменьшить
    static void bindColumnKey(QSqlQuery& query, ColumnKey& columnKey);
    static void bindTaskKey(QSqlQuery& query, TaskKey& taskKey);

    bool isTransaction;
};

#endif // DATABASEMANAGER_H
