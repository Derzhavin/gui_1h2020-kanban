#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "config.h"
#include "databasemanager.h"

#include <QList>
#include <QString>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QDateTime>
#include <QSharedPointer>

struct BoardInfo {
    QString name;
    QString description;
    QString pathToBackGround;
};

struct ColumnInfo {
    QString name;
    QString boardName;
    ColumnUIntT pos;
};

struct TaskInfo {
    QString datetimeCreated;
    QString description;
    QString deadline;
    TaskUIntT pos;
};

using SharedPtrBoardInfoList = QSharedPointer<QList<BoardInfo>>;
using SharedPtrColumnInfoList = QSharedPointer<QList<ColumnInfo>>;
using SharedPtrTaskInfoList = QSharedPointer<QList<TaskInfo>>;

class TaskManager
{
public:
    using OpStatus = DatabaseManager::OpStatus;

    TaskManager();

    SharedPtrBoardInfoList getBoardsInfos();
    QSharedPointer<BoardInfo> getBoard(QString name);
    OpStatus addBoard(QString name, QString descriprion = "", QString pathToBackGround = "");
    OpStatus updateBoard(QString name, QString* newName, QString* newDescription = nullptr, QString* newPathToBackground = nullptr);
    void removeBoard(QString name);

    SharedPtrColumnInfoList getColumnInfosByBoardName(QString boardName);
    QSharedPointer<ColumnInfo> getColumn(QString name);
    OpStatus addColumn(QString name);
    OpStatus updateColumnPos(QString name, ColumnUIntT newPos);
    OpStatus renameColumn(QString name, QString& newColumnName);
    OpStatus removeColumn(QString name);

    SharedPtrTaskInfoList getTaskInfosByBoardColumn(QString boardName, QString columnName);
    OpStatus addTask(QString columnName, QString& datetimeCreated, QString description, QString deadline = "");
    OpStatus updateTaskPosInColumn(QString columnName, QString datetimeCreated, TaskUIntT& newPos);
    OpStatus updateTask(QString columnName, QString datetimeCreated, QString newDescription, QString deadline = "");
    OpStatus moveTaskToOtherColumn(QString columnName, QString datetimeCreated, QString& newColumnName, TaskUIntT& newPos);
    OpStatus removeTask(QString columnName, QString datetimeCreated);

    QString currentBoardName;
};

#endif // TASKMANAGER_H
