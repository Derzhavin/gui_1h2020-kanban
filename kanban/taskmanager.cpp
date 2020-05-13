#include "taskmanager.h"
#include "config.h"
#include <QDebug>

TaskManager::TaskManager(): Singleton<TaskManager>(*this)
{
    database = QSqlDatabase::addDatabase("QSQLITE");

    if (!QFile::exists(DB_DEFAULT_PATH)) {
        QString script;

        QFile file(DB_CREATE_SCRIPT_PATH);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
               return;

        while (!file.atEnd()) {
            script += file.readLine();
        }

        SchemaCreateQuery schemaCreateQuery = SchemaCreateQuery(script);

        if (database.open()) {
            schemaCreateQuery.exec();
        } else {
            QMessageBox::critical(nullptr, "", DB_FAIL_MSG);
        }
    } else {
        database.setDatabaseName(DB_DEFAULT_PATH);
        database.open();
    }
}

bool TaskManager::addBoard(QString &boardName, QString *description, QString *pathToBackground)
{
    query.prepare("INSERT INTO board (name, description, path_to_background) VALUES (?, ?, ?)");

    query.addBindValue(boardName);
    query.addBindValue(description ? *description: nullptr);
    query.addBindValue(pathToBackground ? *pathToBackground: nullptr);

    return query.exec();
}

bool TaskManager::updateBoard(QString &boardName, QString* newBoardName, QString *newDescription, QString *newPathToBackground)
{
    if (newBoardName and newDescription and newPathToBackground) {
        query.prepare("UPDATE board SET name = ?, description = ?, path_to_background = ? WHERE name = ?");

        query.addBindValue(*newBoardName);
        query.addBindValue(*newDescription);
        query.addBindValue(*newPathToBackground);
    }
    else if (newBoardName and newDescription) {
        query.prepare("UPDATE board SET name = ?, description = ? WHERE name = ?");

        query.addBindValue(*newBoardName);
        query.addBindValue(*newDescription);
    }
    else if (newDescription and newPathToBackground) {
        query.prepare("UPDATE board SET description = ?, path_to_background = ? WHERE name = ?");

        query.addBindValue(*newDescription);
        query.addBindValue(*newPathToBackground);
    }
    else if (newBoardName and newPathToBackground) {
        query.prepare("UPDATE board SET name = ?, path_to_background = ? WHERE name = ?");

        query.addBindValue(*newBoardName);
        query.addBindValue(*newPathToBackground);
    }
    else if (newBoardName) {

        query.prepare("UPDATE board SET name = ? WHERE name = ?");
        query.addBindValue(*newBoardName);
    }
    else if (newDescription) {
        query.prepare("UPDATE board SET description = ? WHERE name = ?");

        query.addBindValue(*newDescription);
    }
    else if (newPathToBackground) {
        query.prepare("UPDATE board SET path_to_background = ? WHERE name = ?");

        query.addBindValue(*newPathToBackground);
    }

    query.addBindValue(boardName);

    return query.exec();
}

bool TaskManager::removeBoard(QString &boardName)
{
    query.prepare("DELETE FROM board WHERE name = ?");

    query.addBindValue(boardName);

    return query.exec();
}

bool TaskManager::addColumn(ColumnKey &columnKey, quint8 &pos)
{
    query.prepare("INSERT INTO column (board_name, name, order_num) VALUES (?, ?, ?)");

    query.addBindValue(columnKey.boardName);
    query.addBindValue(columnKey.columName);
    query.addBindValue(pos);

    return query.exec();
}

bool TaskManager::updateColumn(ColumnKey& columnKey, QString* newColumnName, quint8 newPos)
{

}

bool TaskManager::removeColumn(ColumnKey &columnKey)
{
    query.prepare("DELETE FROM column WHERE name = ? and board_name = ? ");

    query.addBindValue(columnKey.columName);
    query.addBindValue(columnKey.boardName);

    return query.exec();
}

bool TaskManager::addTask(TaskKey &taskKey, QString &description, quint8& pos, QString *deadline)
{
    query.prepare("INSERT INTO task"
                  "(column_name, board_name, datetime_created, description, order_num, deadline)"
                  "VALUES (?, ?, ?, ?, ?, ?)");

    query.addBindValue(taskKey.columName);
    query.addBindValue(taskKey.boardName);
    query.addBindValue(taskKey.datetime_created);
    query.addBindValue(description);
    query.addBindValue(pos);
    query.addBindValue(*deadline);

    return query.exec();
}

bool TaskManager::updateTask(TaskKey &taskKey, QString *newDescription, QString *newDeadline, QString *newColumnName, quint8 newPos)
{

}

bool TaskManager::removeTask(TaskKey &taskKey)
{
    query.prepare("DELETE FROM task WHERE column_name = ? and board_name = ? and datetime_created = ?");

    query.addBindValue(taskKey.columName);
    query.addBindValue(taskKey.boardName);
    query.addBindValue(taskKey.datetime_created);

    return query.exec();
}
