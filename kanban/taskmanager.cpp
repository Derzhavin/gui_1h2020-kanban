#include "taskmanager.h"

TaskManager::TaskManager()
{
    DatabaseManager::instance();
}

QSharedPointer<BoardLoad> TaskManager::loadBoard()
{
    BoardLoad *boardLoad= new BoardLoad;
    boardLoad->boardInfo = *(getBoard(currentBoardName).data());

    SharedPtrColumnInfoList sharedPtrColumnInfoList = getColumnInfosByBoardName(currentBoardName);

    if (sharedPtrColumnInfoList.data()) {
        for(ColumnUIntT i = 0; i < sharedPtrColumnInfoList.data()->size(); i++) {
            Column column;
            column.columnInfo = sharedPtrColumnInfoList.data()->at(i);

            SharedPtrTaskInfoList sharedPtrTaskInfoList = getTaskInfosByBoardAndColumn(currentBoardName, column.columnInfo.name);

            if (sharedPtrTaskInfoList .data()) {
                for(TaskUIntT j = 0; j < sharedPtrTaskInfoList.data()->size(); j++) {
                    column.tasks.push_back(sharedPtrTaskInfoList.data()->at(j));
                }
            }
            boardLoad->columns.push_back(column);
        }
    }
    return QSharedPointer<BoardLoad>(boardLoad);
}

SharedPtrBoardList TaskManager::getBoards()
{
    QSqlTableModel model;
    DatabaseManager::instance().selectBoards(model);

    BoardList *boards = new BoardList;
    DatabaseManager::instance().foreachRecordInModel(model, [&](QSqlRecord& record){
        QString board = record.value("name").toString();
        boards->push_back(board);
    });
    return SharedPtrBoardList(boards);
}

QSharedPointer<BoardInfo> TaskManager::getBoard(QString name)
{
    QSqlRecord record = DatabaseManager::instance().selectBoard(name);

    if (record.value("name").toString().isEmpty()) {
        return QSharedPointer<BoardInfo>();
    }

    BoardInfo* boardInfo = new BoardInfo;
    boardInfo->name = record.value("name").toString();
    boardInfo->pathToBackGround = record.value("pathToBackGround").toString();

    return QSharedPointer<BoardInfo>(boardInfo);
}

SharedPtrColumnInfoList TaskManager::getColumnInfosByBoardName(QString boardName)
{
    QSqlTableModel model;
    DatabaseManager::instance().selectColumnsByBoardName(model, boardName);

    QList<ColumnInfo> *columnInfos = new QList<ColumnInfo>;
    DatabaseManager::instance().foreachRecordInModel(model, [&](QSqlRecord& record){
        ColumnInfo columnInfo;
        columnInfo.name = record.value("name").toString();
        columnInfo.boardName = record.value("board_name").toString();
        columnInfo.pos = record.value("order_num").toUInt() - 1;

        columnInfos->push_back(columnInfo);
    });
    return SharedPtrColumnInfoList(columnInfos);
}

QSharedPointer<ColumnInfo> TaskManager::getColumn(QString name)
{
    ColumnKey columnKey(currentBoardName, name);

    QSqlRecord record = DatabaseManager::instance().selectColumn(columnKey);

    if (record.value("name").toString().isEmpty()) {
        return QSharedPointer<ColumnInfo>();
    }

    ColumnInfo* columnInfo = new ColumnInfo;
    columnInfo->boardName = record.value("board_name").toString();
    columnInfo->name = record.value("name").toString();
    columnInfo->pos = record.value("order_num").toUInt();

    return QSharedPointer<ColumnInfo>(columnInfo);
}

SharedPtrTaskInfoList TaskManager::getTaskInfosByBoardAndColumn(QString boardName, QString columnName)
{
    QSqlTableModel model;
    ColumnKey columnKey(boardName, columnName);
    DatabaseManager::instance().selectTasksByColumn(model, columnKey);

    QList<TaskInfo> *taskInfos = new QList<TaskInfo>;
    DatabaseManager::instance().foreachRecordInModel(model, [&](QSqlRecord& record){
        TaskInfo taskInfo;
        taskInfo.datetimeCreated = record.value("datetime_created").toString();
        taskInfo.deadline = record.value("deadline").toString();
        taskInfo.description= record.value("description").toString();
        taskInfo.pos = record.value("order_num").toUInt() -1;

        taskInfos->push_back(taskInfo);
    });
    return SharedPtrTaskInfoList(taskInfos);
}

TaskManager::OpStatus TaskManager::addBoard(QString name, QString descriprion, QString pathToBackGround)
{
    return DatabaseManager::instance().insertBoard(name,
                                            descriprion.isEmpty()? nullptr: &descriprion,
                                            pathToBackGround.isEmpty()? nullptr: &pathToBackGround);
}

TaskManager::OpStatus TaskManager::addColumn(QString name)
{
    ColumnKey columnKey(currentBoardName, name);
    return DatabaseManager::instance().insertBackColumn(columnKey);
}

TaskManager::OpStatus TaskManager::addTask(QString columnName, QString& datetimeCreated, QString description, QString deadline)
{
    QDateTime datetime = QDateTime::currentDateTime();
    datetimeCreated = datetime.toString("yy-MM-dd hh:mm:ss");

    TaskKey taskKey(currentBoardName, columnName, datetimeCreated);
    return DatabaseManager::instance().insertBackTask(taskKey, description, deadline.isEmpty()? nullptr: &deadline);
}

TaskManager::OpStatus TaskManager::updateBoard(QString *newName, QString *newDescription, QString *newPathToBackground)
{
    return DatabaseManager::instance().updateBoard(currentBoardName, newName, newDescription, newPathToBackground);
}

TaskManager::OpStatus TaskManager::updateColumnPos(QString name, ColumnUIntT newPos)
{
    ColumnKey columnKey(currentBoardName, name);
    return DatabaseManager::instance().updateColumnPosInBoard(columnKey, newPos);
}

TaskManager::OpStatus TaskManager::renameColumn(QString name, QString &newColumnName)
{
    ColumnKey columnKey(currentBoardName, name);
    return DatabaseManager::instance().updateColumnName(columnKey, newColumnName);
}

TaskManager::OpStatus TaskManager::updateTask(QString columnName, QString datetimeCreated, QString newDescription, QString deadline)
{
    TaskKey taskKey(currentBoardName, columnName, datetimeCreated);
    return DatabaseManager::instance().updateTask(taskKey, newDescription, deadline.isEmpty() ? nullptr: &deadline);
}

TaskManager::OpStatus TaskManager::updateTaskPosInColumn(QString columnName, QString datetimeCreated, TaskUIntT &newPos)
{
    TaskKey taskKey(currentBoardName, columnName, datetimeCreated);
    return DatabaseManager::instance().updateTaskPosInColumn(taskKey, newPos);
}

TaskManager::OpStatus TaskManager::moveTaskToOtherColumn(QString columnName, QString datetimeCreated, QString &newColumnName, TaskUIntT &newPos)
{
    TaskKey taskKey(currentBoardName, columnName, datetimeCreated);
    return DatabaseManager::instance().moveTaskToOtherColumn(taskKey, newColumnName, newPos);
}

TaskManager::OpStatus TaskManager::removeBoard()
{
    return DatabaseManager::instance().deleteBoard(currentBoardName);
}

TaskManager::OpStatus TaskManager::removeColumn(QString name)
{
    ColumnKey columnKey(currentBoardName, name);
    return DatabaseManager::instance().deleteColumn(columnKey);
}

TaskManager::OpStatus TaskManager::removeTask(QString columnName, QString datetimeCreated)
{
    TaskKey taskKey(currentBoardName, columnName, datetimeCreated);
    return DatabaseManager::instance().deleteTask(taskKey);
}

