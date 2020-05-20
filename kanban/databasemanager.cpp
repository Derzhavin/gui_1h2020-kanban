#include "databasemanager.h"
#include <QDebug>

DatabaseManager::DatabaseManager(): Singleton<DatabaseManager>(*this), isTransaction(false)
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

DatabaseManager::OpStatus DatabaseManager::insertBoard(QString &boardName, QString *description, QString *pathToBackground)
{
    if (boardName.size() > DB_MAX_BOARD_NAME) {
        return OpStatus::LongBoardName;
    }

    if (description and description->size() > DB_MAX_BOARD_DESCRIPTION) {
        return OpStatus::LongBoardDescription;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO board(name, description, path_to_background) VALUES (?, ?, ?)");
    query.addBindValue(boardName);
    query.addBindValue(description ? *description: nullptr);
    query.addBindValue(pathToBackground ? *pathToBackground: nullptr);

    return query.exec() ? OpStatus::Success : OpStatus::Failure;
}

DatabaseManager::OpStatus DatabaseManager::updateBoard(QString &boardName, QString* newBoardName, QString *newDescription, QString *newPathToBackground)
{
    QSqlQuery query;

    if (newBoardName and newBoardName->size() > DB_MAX_BOARD_NAME) {
        return OpStatus::LongBoardName;
    }

    if (newDescription and newDescription->size() > DB_MAX_BOARD_DESCRIPTION) {
        return OpStatus::LongBoardDescription;
    }

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

    return query.exec() ? OpStatus::Success : OpStatus::Failure;
}

bool DatabaseManager::deleteBoard(QString &boardName)
{
    QSqlQuery query;
    query.prepare("DELETE FROM board WHERE name = ?");
    query.addBindValue(boardName);

    return query.exec();
}

void DatabaseManager::selectBoards(QSqlTableModel& model)
{
    selectFromTable(model, "board", [&](){});
}

QSqlRecord DatabaseManager::selectBoard(QString &boardName)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM board WHERE name = ?");
    query.addBindValue(boardName);
    if (query.exec()) {
        query.next();
        return query.record();
    }

    return QSqlRecord();
}

DatabaseManager::OpStatus DatabaseManager::insertBackColumn(ColumnKey &columnKey)
{
    return doTransaction([&]() {
        ColumnUIntT maxColumnPos = findMaxColumnPosInBoard(columnKey.first);

        if (maxColumnPos + 1 > DB_MAX_COLUMN_NUM) {
            return OpStatus::TableFull;
        }
        return insertColumn(columnKey, maxColumnPos + 1);
    });
}


DatabaseManager::OpStatus DatabaseManager::insertColumn(ColumnKey &columnKey, ColumnUIntT pos)
{
    if (columnKey.second > DB_MAX_COLUMN_NAME) {
        return OpStatus::LongColumnName;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO column (board_name, name, order_num) VALUES (?, ?, ?)");
    bindColumnKey(query, columnKey);
    query.addBindValue(pos);
    return query.exec() ? OpStatus::Success : OpStatus::Failure;
}

DatabaseManager::OpStatus DatabaseManager::updateColumnPosInBoard(ColumnKey& columnKey, ColumnUIntT &newPos)
{
    return doTransaction([&]() {
        ColumnUIntT prevPos = selectColumn(columnKey).value("order_num").toUInt();

        if (!prevPos) {
            return  OpStatus::NoColumn;
        }

        QSqlQuery query;

        query.prepare("UPDATE column SET order_num = order_num + ? WHERE board_name = ? AND (order_num BETWEEN ? AND ?)");
        query.addBindValue(newPos > prevPos ? -1: 1);
        query.addBindValue(columnKey.first);
        query.addBindValue(QString::number(std::min(prevPos, newPos)));
        query.addBindValue(QString::number(std::max(prevPos, newPos)));

        if (query.exec()) {
            return updateColumnPos(columnKey, newPos) ? OpStatus::Success: OpStatus::Failure;
        }

        return OpStatus::Failure;
    });
}

DatabaseManager::OpStatus DatabaseManager::updateColumnName(ColumnKey& columnKey, QString& newColumnName)
{
    if (newColumnName.size() > DB_MAX_COLUMN_NAME) {
        return OpStatus::LongColumnName;
    }

    QSqlQuery query;
    query.prepare("UPDATE column SET name = ? WHERE board_name = ? AND name = ?");
    query.addBindValue(newColumnName);
    bindColumnKey(query, columnKey);

    return query.exec() ? OpStatus::Success : OpStatus::Failure;
}

DatabaseManager::OpStatus DatabaseManager::deleteColumn(ColumnKey &columnKey)
{
    return doTransaction([&] () {
        ColumnUIntT pos = selectColumn(columnKey).value("order_num").toUInt();

        if (pos) {
            QSqlQuery query;
            query.prepare("DELETE FROM column WHERE board_name = ? AND name = ?");
            bindColumnKey(query, columnKey);

            if (query.exec()) {
                query.prepare("UPDATE column SET order_num = order_num - 1 "
                              "WHERE board_name = ? AND (order_num BETWEEN ? AND ?)");
                query.addBindValue(columnKey.first);
                query.addBindValue(pos);
                query.addBindValue(DB_MAX_COLUMN_NUM);
                return query.exec() ? OpStatus::Success : OpStatus::Failure;
            }

            return OpStatus::Failure;
        }

        return OpStatus::NoColumn;
    });
}

void DatabaseManager::selectColumnsByBoardName(QSqlTableModel &model, QString &boardName)
{
    selectFromTable(model, "column", [&]() {model.setFilter("board_name = " + boardName);});
}

QSqlRecord DatabaseManager::selectColumn(ColumnKey &columnKey)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM column WHERE board_name = ? AND name = ?");
    bindColumnKey(query, columnKey);
    if (query.exec()) {
        query.next();
        return query.record();
    }

    return QSqlRecord();
}

ColumnUIntT DatabaseManager::findMaxColumnPosInBoard(QString boardName)
{
    QSqlQuery query;
    query.prepare("SELECT MAX(order_num) AS max_order_num FROM column WHERE board_name = ?");
    query.addBindValue(boardName);
    if (query.exec()) {
        query.next();
        return query.record().value("max_order_num").toUInt();
    }

    return 0;
}

DatabaseManager::OpStatus DatabaseManager::insertBackTask(TaskKey &taskKey, QString &description, QString *deadline)
{
    ColumnKey columnKey(std::get<0>(taskKey), std::get<1>(taskKey));
    TaskUIntT maxPos = findMaxTaskPosInColumn(columnKey);

    if (maxPos + 1 < DB_MAX_TASK_NUM) {
        return insertTask(taskKey, maxPos + 1, description, deadline);
    }

    return OpStatus::TableFull;
}

DatabaseManager::OpStatus DatabaseManager::insertTask(TaskKey &taskKey, TaskUIntT pos, QString &description, QString *deadline)
{
    if (description.size() > DB_MAX_TASK_DESCRIPTION) {
        return OpStatus::LongTaskDescription;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO task "
                  "("
                  "board_name, "
                  "column_name, "
                  "datetime_created, "
                  "description, "
                  "deadline, "
                  "order_num"
                  ") VALUES (?, ?, ?, ?, ?, ?)");
    bindTaskKey(query, taskKey);
    query.addBindValue(description);
    query.addBindValue(deadline ? *deadline: nullptr);
    query.addBindValue(pos);

    return query.exec() ? OpStatus::Success : OpStatus::Failure;
}

DatabaseManager::OpStatus DatabaseManager::moveTaskToOtherColumn(TaskKey& taskKey, QString& newColumnName, TaskUIntT newPos)
{
    return doTransaction([&]() {
        QSqlRecord record = selectTask(taskKey);

        if (record.count()) {
            OpStatus deleteStatus = deleteTask(taskKey);

            if (deleteStatus == OpStatus::Success) {
                TaskKey newTaskKey(std::get<0>(taskKey), newColumnName, std::get<2>(taskKey));

                QString description = record.value("description").toString();
                QVariant variantDeadline = record.value("deadline");

                OpStatus insertBackTaskStatus = OpStatus::Success;

                if (variantDeadline.isValid()) {
                    QString deadline = variantDeadline.toString();

                    if (!newPos) {
                        return insertBackTask(newTaskKey, description, &deadline);
                    }

                    insertBackTaskStatus = insertBackTask(newTaskKey, description, &deadline);
                } else {
                    if (!newPos) {
                        return insertBackTask(newTaskKey, description, nullptr);
                    }

                    insertBackTaskStatus = insertBackTask(newTaskKey, description, nullptr);
                }

                if (insertBackTaskStatus == OpStatus::Success) {
                    return updateTaskPosInColumn(newTaskKey, newPos);
                }

                return insertBackTaskStatus;
            }
            return deleteStatus;
        }

        return OpStatus::NoTask;
    });
}
DatabaseManager::OpStatus DatabaseManager::updateTaskPosInColumn(TaskKey& taskKey, TaskUIntT &newPos)
{
    return doTransaction([&]() {
        TaskUIntT prevPos = selectTask(taskKey).value("order_num").toUInt();

        if (!prevPos) {
            return OpStatus::NoTask;
        }

        QSqlQuery query;
        query.prepare("UPDATE task SET order_num = order_num + ? "
                      "WHERE board_name = ? "
                      "AND "
                      "column_name = ? "
                      "AND "
                      "(order_num BETWEEN ? AND ?)");

        query.addBindValue(newPos > prevPos ? -1: 1);
        query.addBindValue(std::get<0>(taskKey));
        query.addBindValue(std::get<1>(taskKey));
        query.addBindValue(QString::number(std::min(prevPos, newPos)));
        query.addBindValue(QString::number(std::max(prevPos, newPos)));

        if (query.exec()) {
            return updateTaskPos(taskKey, newPos) ? OpStatus::Success : OpStatus::Failure;
        }

        return OpStatus::Failure;
    });
}

bool DatabaseManager::updateTaskPos(TaskKey &taskKey, TaskUIntT pos)
{
    QSqlQuery query;
    query.prepare("UPDATE task SET order_num = ? WHERE board_name = ? AND column_name = ? AND datetime_created = ?");
    query.addBindValue(pos);
    bindTaskKey(query, taskKey);

    return query.exec();
}

DatabaseManager::OpStatus DatabaseManager::doTransaction(std::function<OpStatus()> callback)
{
    if (!isTransaction) {
        isTransaction = true;
        database.transaction();
    }

    OpStatus opStatus = callback();

    if (opStatus == OpStatus::Success) {
        database.commit();
        isTransaction = false;
        return OpStatus::Success;
    }

    if (!isTransaction) {
        database.rollback();
        isTransaction = false;
    }

    return opStatus;
}

DatabaseManager::OpStatus DatabaseManager::updateTask(TaskKey& taskKey, QString& newDescription, QString *deadline){
    if (newDescription > DB_MAX_TASK_DESCRIPTION) {
        return OpStatus::LongTaskDescription;
    }

    QSqlQuery query;
    query.prepare("UPDATE task SET description = ?, deadline = ? "
                  "WHERE board_name = ? "
                  "AND "
                  "column_name = ? "
                  "AND "
                  "datetime_created = ?");
    query.addBindValue(newDescription);
    query.addBindValue(deadline ? *deadline: nullptr);
    bindTaskKey(query, taskKey);

    return query.exec() ? OpStatus::Success : OpStatus::Failure;
}

DatabaseManager::OpStatus DatabaseManager::deleteTask(TaskKey &taskKey)
{
    return doTransaction([&]() {
        TaskUIntT pos = selectTask(taskKey).value("order_num").toUInt();

        if (!pos) {
            return OpStatus::NoTask;
        }

        QSqlQuery query;
        query.prepare("DELETE FROM task WHERE board_name = ? AND column_name = ? AND datetime_created = ?");
        bindTaskKey(query, taskKey);

        if (query.exec()) {
            query.prepare("UPDATE task SET order_num = order_num + ? "
                          "WHERE board_name = ? "
                          "AND "
                          "column_name = ? "
                          "AND "
                          "(order_num BETWEEN ? AND ?)");

            query.addBindValue(-1);
            query.addBindValue(std::get<0>(taskKey));
            query.addBindValue(std::get<1>(taskKey));
            query.addBindValue(pos);
            query.addBindValue(DB_MAX_TASK_NUM);

            return query.exec() ? OpStatus::Success : OpStatus::Failure;
        }

        return OpStatus::Failure;
    });
}

void DatabaseManager::foreachRecordInModel(QSqlTableModel &model, std::function<void (QSqlRecord &)> callback)
{
    for (RecordUIntT i = 0; i < model.rowCount(); ++i) {
        QSqlRecord record = model.record(i);
        callback(record);
        model.setRecord(i, record);
    }
}

void DatabaseManager::selectFromTable(QSqlTableModel& model,QString tableName, std::function<void()> callback)
{
    model.setTable(tableName);
    callback();
    model.select();
}

bool DatabaseManager::updateColumnPos(ColumnKey &columnKey, ColumnUIntT &newPos)
{
    QSqlQuery query;
    query.prepare("UPDATE column SET order_num = ? WHERE board_name = ? AND name = ?");
    query.addBindValue(newPos);
    bindColumnKey(query, columnKey);

    return query.exec();
}

void DatabaseManager::moveRowsByOne(QSqlTableModel &model, bool direction)
{
    foreachRecordInModel(model, [&](QSqlRecord& record) {
        RecordUIntT pos = record.value("order_num").toUInt();
        pos += direction ? -1: 1;
        record.setValue("order_num", pos);
    });
}

TaskUIntT DatabaseManager::findMaxTaskPosInColumn(ColumnKey& columnKey)
{
    QSqlQuery query;
    query.prepare("SELECT MAX(order_num) AS max_order_num FROM task WHERE board_name = ? AND column_name = ?");
    bindColumnKey(query, columnKey);
    if (query.exec()) {
        query.next();
        return query.record().value("max_order_num").toUInt();
    }

    return 0;
}

void DatabaseManager::selectTasksByColumn(QSqlTableModel &model, ColumnKey &columnKey)
{
    selectFromTable(model, "column", [&]() {
        model.setFilter("board_name = " + columnKey.first + "name = " + columnKey.second);
    });
}

QSqlRecord DatabaseManager::selectTask(TaskKey &taskKey)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM task WHERE board_name = ? AND column_name = ? AND datetime_created = ?");
    bindTaskKey(query, taskKey);
    if (query.exec()) {
        query.next();
        return query.record();
    }

    return QSqlRecord();
}

void DatabaseManager::bindColumnKey(QSqlQuery &query, ColumnKey &columnKey)
{
    query.addBindValue(columnKey.first);
    query.addBindValue(columnKey.second);
}

void DatabaseManager::bindTaskKey(QSqlQuery &query, TaskKey &taskKey)
{
    query.addBindValue(std::get<0>(taskKey));
    query.addBindValue(std::get<1>(taskKey));
    query.addBindValue(std::get<2>(taskKey));
}

