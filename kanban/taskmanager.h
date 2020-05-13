#ifndef TASKMANAGER_H
#define TASKMANAGER_H


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
    QString datetime_created;
};

class TaskManager: public Singleton<TaskManager>
{

public:
    TaskManager();

    bool addBoard(QString& boardName, QString* description = nullptr, QString* pathToBackground = nullptr);
    bool updateBoard(QString& boardName, QString* newBoardName, QString* newDescription = nullptr, QString* newPathToBackground = nullptr);
    bool removeBoard(QString& boardName);

    bool addColumn(ColumnKey& columnKey, quint8& pos);
    bool updateColumn(ColumnKey& columnKey, QString* newColumnName = nullptr, quint8 newPos = -1);
    bool removeColumn(ColumnKey& columnKey);

    bool addTask(TaskKey& taskKey, QString& description, quint8& pos, QString* deadline = nullptr);
    bool updateTask(TaskKey& taskKey, QString* newDescription = nullptr, QString* newDeadline = nullptr, QString* newColumnName = nullptr, quint8 newPos = -1);
    bool removeTask(TaskKey& taskKey);

private:
    QSqlDatabase database;
    QSqlQuery query;
};

#endif // TASKMANAGER_H
