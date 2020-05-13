#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H


#include "singleton.h"
#include "schemacreatequery.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>
#include <QFile>
#include <QMessageBox>
#include <QString>

typedef struct ColumnKey ColumnKey;
struct ColumnKey {
    QString boardName;
    QString columName;
};

typedef struct TaskKey TaskKey;
struct TaskKey {
    QString boardName;
    QString columName;
    quint8 pos;
};

class DatabaseManager: public Singleton<DatabaseManager>
{
    Q_ENUMS(DatabaseQueryStatus)

public:
    DatabaseManager();

    enum class DatabaseQueryStatus
    {
        Success,
        ItemExist,
        Failure
    };

    DatabaseQueryStatus addBoard(QString& boardName, QString* description = nullptr, QString* pathToBackground = nullptr);
    DatabaseQueryStatus updateBoard(QString& boardName, QString* newBoardName, QString* newDescription = nullptr, QString* newPathToBackground = nullptr);
    DatabaseQueryStatus removeBoard(QString& boardName);

    DatabaseQueryStatus addColumn(ColumnKey& columnKey, quint8& pos);
    DatabaseQueryStatus updateColumn(ColumnKey& columnKey, QString* newColumnName = nullptr, quint8 newPos = -1);
    DatabaseQueryStatus removeColumn(ColumnKey& columnKey);

    DatabaseQueryStatus addTask(TaskKey& taskKey, QString& description, QString& datetimeCreated, QString* deadline = nullptr);
    DatabaseQueryStatus updateTask(TaskKey& taskKey, QString* newDescription = nullptr, QString* newDeadline = nullptr, QString* newColumnName = nullptr, quint8 newPos = -1);
    DatabaseQueryStatus removeTask(TaskKey& taskKey);

private:
    QSqlDatabase database;
};

#endif // DATABASEMANAGER_H
