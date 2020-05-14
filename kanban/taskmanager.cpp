#include "taskmanager.h"

TaskManager::TaskManager()
{

}

QList<BoardInfo> TaskManager::getBoardsInfos()
{
    QSqlTableModel model;
    DatabaseManager::instance().selectBoards(model);

    QList<BoardInfo> boardsInfos;
    foreachRecord(model, [&](QSqlRecord& record){
        BoardInfo boardInfo;
        boardInfo.name = record.value("name").toString();
        boardInfo.pathToBackGround = record.value("pathToBackGround").toString();
        boardInfo.description = record.value("description").toString();

        boardsInfos.push_back(boardInfo);
    });
    return boardsInfos;
}

QList<ColumnInfo> TaskManager::getColumnInfosByBoardName(QString boardName)
{
    QSqlTableModel model;
    DatabaseManager::instance().selectColumnsByBoardName(model, boardName);

    QList<ColumnInfo> columnInfos;
    foreachRecord(model, [&](QSqlRecord& record){
        ColumnInfo columnInfo;
        columnInfo.name = record.value("name").toString();
        columnInfo.boardName = record.value("board_name").toString();
        columnInfo.pos = record.value("order_num").toUInt();

        columnInfos.push_back(columnInfo);
    });
    return columnInfos;
}

QList<TaskInfo> TaskManager::getTaskInfosByBoardNameAndColumnName(QString boardName, QString columnName)
{
    QSqlTableModel model;
    DatabaseManager::instance().selectTasksByBoardNameAndColumnName(model, boardName, columnName);

    QList<TaskInfo> taskInfos;
    foreachRecord(model, [&](QSqlRecord& record){
        TaskInfo taskInfo;
        taskInfo.datetimeCreated = record.value("datetime_created").toString();
        taskInfo.deadline = record.value("deadline").toString();
        taskInfo.description= record.value("description").toString();
        taskInfo.pos = record.value("order_num").toUInt();

        taskInfos.push_back(taskInfo);
    });
    return taskInfos;
}

void TaskManager::addBoard(QString name, QString descriprion, QString pathToBackGround)
{
    DatabaseManager::instance().insertBoard(name, descriprion.isEmpty()? nullptr: &descriprion, pathToBackGround.isEmpty()? nullptr: &pathToBackGround);
}

void TaskManager::addColumn(QString name, quint8 pos)
{
    ColumnKey columnKey(currentBoardName, name);
    DatabaseManager::instance().insertColumn(columnKey, pos);
}

void TaskManager::addTask(QString columnName, QString description, quint8 &pos, QString deadline)
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString datetimeCreated = datetime.toString("yy-MM-dd hh:mm::ss");

    TaskKey taskKey(currentBoardName, columnName, datetimeCreated);
    DatabaseManager::instance().insertTask(taskKey, description, pos, deadline.isEmpty()? nullptr: &deadline);
}

void TaskManager::updateBoard(QString name, QString *newName, QString *newDescription, QString *newPathToBackground)
{
    DatabaseManager::instance().updateBoard(name, newName, newDescription, newPathToBackground);
}

void TaskManager::updateColumnPos(QString name, quint8 prevPos, quint8 newPos)
{
    ColumnKey columnKey(currentBoardName, name);
    DatabaseManager::instance().updateColumnPos(columnKey, prevPos, newPos);
}

void TaskManager::updateColumnName(QString name, QString &newColumnName)
{
    ColumnKey columnKey(currentBoardName, name);
    DatabaseManager::instance().updateColumnName(columnKey, newColumnName);
}

void TaskManager::updateTaskDescription(QString columnName, QString datetimeCreated, QString newDescription)
{
    TaskKey taskKey(currentBoardName, columnName, datetimeCreated);
    DatabaseManager::instance().updateTaskDescription(taskKey, newDescription);
}

void TaskManager::updateTaskPosInColumn(QString columnName, QString datetimeCreated, quint8 &prevPos, quint8 &newPos)
{
    TaskKey taskKey(currentBoardName, columnName, datetimeCreated);
    DatabaseManager::instance().updateTaskPosInColumn(taskKey, prevPos, newPos);
}

void TaskManager::moveTaskToOtherColumn(QString columnName, QString datetimeCreated, QString &newColumnName, quint8 &prevPos, quint8 &newPos)
{
    TaskKey taskKey(currentBoardName, columnName, datetimeCreated);
    DatabaseManager::instance().moveTaskToOtherColumn(taskKey, newColumnName, prevPos, newPos);
}

void TaskManager::removeBoard(QString name)
{
    DatabaseManager::instance().deleteBoard(name);
}

void TaskManager::removeColumn(QString name, quint8 &prevPos)
{
    ColumnKey columnKey(currentBoardName, name);
    DatabaseManager::instance().deleteColumn(columnKey, prevPos);
}

void TaskManager::removeTask(QString name, QString columnName, QString datetimeCreated, quint8 prevPos)
{
    TaskKey taskKey(currentBoardName, columnName, datetimeCreated);
    DatabaseManager::instance().deleteTask(taskKey, prevPos);
}

