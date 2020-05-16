#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "databasemanager.h"
#include "sqltablemodelextension.h"

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
    quint8 pos;
};

struct TaskInfo {
    QString datetimeCreated;
    QString description;
    QString deadline;
    quint8 pos;
};

class TaskManager
{
public:
    TaskManager();

    QList<BoardInfo> getBoardsInfos();
    QSharedPointer<BoardInfo> getBoard(QString name);
    void addBoard(QString name, QString descriprion = "", QString pathToBackGround = "");
    void updateBoard(QString name, QString* newName, QString* newDescription = nullptr, QString* newPathToBackground = nullptr);
    void removeBoard(QString name);

    QList<ColumnInfo> getColumnInfosByBoardName(QString boardName);
    QSharedPointer<ColumnInfo> getColumn(QString name);
    void addColumn(QString name);
    void updateColumnPos(QString name, quint8 prevPos, quint8 newPos);
    void updateColumnName(QString name, QString& newColumnName);
    void removeColumn(QString name, quint8 &prevPos);

    QList<TaskInfo> getTaskInfosByBoardNameAndColumnName(QString boardName, QString columnName);
    void addTask(QString columnName, QString description, quint8& pos, QString deadline = "");
    void updateTaskPosInColumn(QString columnName, QString datetimeCreated, quint8& prevPos, quint8& newPos);
    void updateTaskDescription(QString columnName, QString datetimeCreated, QString newDescription);
    void moveTaskToOtherColumn(QString columnName, QString datetimeCreated, QString& newColumnName, quint8& prevPos, quint8& newPos);
    void removeTask(QString columnName, QString datetimeCreated, quint8 prevPos);

    QString currentBoardName;
};

#endif // TASKMANAGER_H
