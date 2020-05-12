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
    DatabaseQueryStatus removeBoard(QString& boardName);
    DatabaseQueryStatus updateBoardName(QString& prevBoardName, QString& newBoardName);
    DatabaseQueryStatus updateBoardDescriprion(QString& boardName, QString& newDescription);
    DatabaseQueryStatus updateBoardBackGround(QString& boardName, QString& pathToBackground);

    DatabaseQueryStatus addColumn(QString& boardName, QString& columnName, quint8 pos = 0);
    DatabaseQueryStatus removeColumn(QString& boardName, QString& columnName);
    DatabaseQueryStatus updateColumnName(QString& boardName, QString& prevColumnName, QString& newColumnName);
    DatabaseQueryStatus updateColumnPos(QString& boardName, QString& columnName, quint8& prevPos, quint8& newPos);

    DatabaseQueryStatus addTask(QString& boardName, QString& columnName, QString& description, QString& datetimeCreated, QString* deadline = nullptr, quint8 pos = 0);
    DatabaseQueryStatus removeTask(QString& boardName, QString& columnName, quint8& pos);
    DatabaseQueryStatus updateTaskDescription(QString& boardName, QString& columnName, quint8& pos, QString& newDescription);
    DatabaseQueryStatus updateTaskDeadline(QString& newDeadline);
    DatabaseQueryStatus updateTaskPos(QString& boardName, QString& columnName, quint8& prevPos, quint8& newPos, QString* newColumnName = nullptr);

private:
    QSqlDatabase database;
};

#endif // DATABASEMANAGER_H
