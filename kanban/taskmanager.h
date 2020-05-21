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
#include <QPair>

struct BoardInfo {
    QString name;
    QString pathToBackGround;
    QString description;
};

class Cache
{
public:
    Cache() {}

    void cacheBoardInfo(BoardInfo boardInfo) {
        this->boardInfo = boardInfo;
    }
    BoardInfo getCachedBoardInfo() {
        return boardInfo;
    }
    QString getCachedBoardName() {
        return boardInfo.name;
    }

private:
    BoardInfo boardInfo;
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

struct Column;

using SharedPtrBoardInfoList = QSharedPointer<QList<BoardInfo>>;
using SharedPtrColumnInfoList = QSharedPointer<QList<ColumnInfo>>;
using SharedPtrTaskInfoList = QSharedPointer<QList<TaskInfo>>;
using SharedPtrBoardList = QSharedPointer<QList<QString>>;
using BoardList = QList<QString>;
using Tasks = QVector<TaskInfo>;
using Columns = QList<Column>;

struct Column {
    ColumnInfo columnInfo;
    Tasks tasks;
};

struct BoardLoad {
    BoardInfo boardInfo;
    Columns columns;
};

class TaskManager
{
public:
    using OpStatus = DatabaseManager::OpStatus;

    TaskManager();

    QSharedPointer<BoardLoad> loadBoard();

    SharedPtrBoardList getBoards();
    QSharedPointer<BoardInfo> getBoardInfo(QString name = "");
    OpStatus addBoard(QString name, QString descriprion = "", QString pathToBackGround = "");
    OpStatus updateBoard(QString newName = "", QString newDescription = "", QString newPathToBackground = "");
    OpStatus removeBoard();

    SharedPtrColumnInfoList getColumnInfosByBoardName(QString boardName);
    QSharedPointer<ColumnInfo> getColumn(QString name);
    OpStatus addColumn(QString name);
    OpStatus updateColumnPos(QString name, ColumnUIntT newPos);
    OpStatus renameColumn(QString name, QString& newColumnName);
    OpStatus removeColumn(QString name);

    SharedPtrTaskInfoList getTaskInfosByBoardAndColumn(QString boardName, QString columnName);
    OpStatus addTask(QString columnName, QString& datetimeCreated, QString description, QString deadline = "");
    OpStatus updateTaskPosInColumn(QString columnName, QString datetimeCreated, TaskUIntT& newPos);
    OpStatus updateTask(QString columnName, QString datetimeCreated, QString newDescription, QString deadline = "");
    OpStatus moveTaskToOtherColumn(QString columnName, QString datetimeCreated, QString& newColumnName, TaskUIntT& newPos);
    OpStatus removeTask(QString columnName, QString datetimeCreated);

    QString currentBoardName;
    Cache cache;
};

#endif // TASKMANAGER_H
