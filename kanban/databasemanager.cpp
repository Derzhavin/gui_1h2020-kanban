#include "databasemanager.h"
#include "config.h"
#include <QDebug>

DatabaseManager::DatabaseManager(): Singleton<DatabaseManager>(*this)
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

bool DatabaseManager::insertBoard(QString &boardName, QString *description, QString *pathToBackground)
{
    QSqlQuery query;
    query.prepare("INSERT INTO board (name, description, path_to_background) VALUES (?, ?, ?)");
    query.addBindValue(boardName);
    query.addBindValue(description ? *description: nullptr);
    query.addBindValue(pathToBackground ? *pathToBackground: nullptr);

    return query.exec();
}

bool DatabaseManager::updateBoard(QString &boardName, QString* newBoardName, QString *newDescription, QString *newPathToBackground)
{
    QSqlQuery query;

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

bool DatabaseManager::insertBackColumn(ColumnKey &columnKey)
{
    quint8 maxColumnPos = findMaxColumnPosInBoard(columnKey.first);

    QSqlQuery query;
    query.prepare("INSERT INTO column (board_name, name, order_num) VALUES (?, ?, ?)");
    bindColumnKey(query, columnKey);
    query.addBindValue(maxColumnPos + 1);

    return query.exec();
}

bool DatabaseManager::updateColumnPos(ColumnKey& columnKey, quint8& newPos)
{
    quint8 prevPos = selectColumn(columnKey).value("order_num").toUInt();

    QSqlTableModel model;
    QString filter = "board_name = " + columnKey.first +
                     "name = " + columnKey.second;

    selectFromTable(model, "column", [&]() {
        model.setSort(model.fieldIndex("order_num"), Qt::AscendingOrder);
        model.setFilter(filter);
    });
    moveRowsByOne(model, newPos < prevPos);

    quint8 recorInd = newPos > prevPos ? 0: model.rowCount();
    QSqlRecord record = model.record(recorInd);
    record.setValue("order_num", newPos);
    model.setRecord(recorInd, record);

    return model.submitAll();
}

bool DatabaseManager::updateColumnName(ColumnKey& columnKey, QString& newColumnName)
{
    QSqlQuery query;
    query.prepare("UPDATE column SET name = ? WHERE board_name = ? AND name = ?");
    query.addBindValue(newColumnName);
    bindColumnKey(query, columnKey);

    return query.exec();
}

bool DatabaseManager::deleteColumn(ColumnKey &columnKey)
{
    QSqlTableModel model;
    QString filter = "board_name = " + columnKey.first +
                     "name = " + columnKey.second;

    selectFromTable(model, "column", [&]() {
        model.setSort(model.fieldIndex("order_num"), Qt::AscendingOrder);
        model.setFilter(filter);
    });
    moveRowsByOne(model, false);
    model.removeRows(0, 1);

    return model.submitAll();
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

quint8 DatabaseManager::findMaxColumnPosInBoard(QString boardName)
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

bool DatabaseManager::insertBackTask(TaskKey &taskKey, QString &description, QString *deadline)
{
    QSqlTableModel model;
    ColumnKey columnKey(std::get<0>(taskKey), std::get<1>(taskKey));
    quint8 maxPos = findMaxTaskPosInColumn(columnKey);

    QSqlQuery query;
    query.prepare("INSERT INTO column ("
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
    query.addBindValue(maxPos + 1);

    return query.exec();

}

bool DatabaseManager::moveTaskToOtherColumn(TaskKey& taskKey, QString& newColumnName, quint8& newPos)
{
//    QSqlRecord record = selectTask(taskKey);

//    if (deleteTask(taskKey, prevPos)) {
//        taskKey = TaskKey(std::get<0>(taskKey), newColumnName, std::get<2>(taskKey));
//        QString description = record.value("description").toString();
//        QVariant variant = record.value("deadline");

//        if (variant.isValid()) {
//            QString description = variant.toString();
//            return insertTaskFront(taskKey, description, newPos, &description);
//        }

//        return insertTaskFront(taskKey, description, newPos, nullptr);
//    }
    return false;
}
bool DatabaseManager::updateTaskPosInColumn(TaskKey& taskKey, quint8& newPos)
{
    quint8 prevPos = selectTask(taskKey).value("order_num").toUInt();

    QSqlTableModel model;

    QString filter = "board_name = " + std::get<0>(taskKey) +
                     " AND "
                     "column_name = " + std::get<1>(taskKey) +
                     "WHERE"
                     "order_num BETWEEN " +
                     QString::number(std::min(newPos, prevPos)) +
                     " AND " +
                     QString::number(std::max(newPos, prevPos));

    selectFromTable(model, "task", [&]() {
        model.setSort(model.fieldIndex("order_num"), Qt::AscendingOrder);
        model.setFilter(filter);
    });
    moveRowsByOne(model, newPos < prevPos);

    quint8 recorInd = newPos > prevPos ? 0: model.rowCount();
    QSqlRecord record = model.record(recorInd);
    record.setValue("order_num", newPos);
    model.setRecord(recorInd, record);

    return model.submitAll();
}
bool DatabaseManager::updateTaskDescription(TaskKey& taskKey, QString& newDescription){
    QSqlQuery query;
    query.prepare("UPDATE task SET description = ? WHERE board_name = ? AND column_name = ? AND datetime_created = ?");
    query.addBindValue(newDescription);
    bindTaskKey(query, taskKey);

    return query.exec();
}

bool DatabaseManager::deleteTask(TaskKey &taskKey)
{
    quint8 prevPos = selectTask(taskKey).value("order_num").toUInt();

    QSqlTableModel model;

    QString filter = "board_name = " + std::get<0>(taskKey) +
                     " AND "
                     "column_name = " + std::get<1>(taskKey) +
                     "WHERE"
                     "order_num BETWEEN " +
                     QString::number(prevPos) +
                     " AND " +
                     QString::number(255);

    selectFromTable(model, "task", [&]() {
        model.setSort(model.fieldIndex("order_num"), Qt::AscendingOrder);
        model.setFilter(filter);
    });
    moveRowsByOne(model, false);
    model.removeRows(0, 1);

    return model.submitAll();
}

void DatabaseManager::foreachRecordInModel(QSqlTableModel &model, std::function<void (QSqlRecord &)> callback)
{
    for (quint8 i = 0; i < model.rowCount(); ++i) {
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

void DatabaseManager::moveRowsByOne(QSqlTableModel &model, bool direction)
{
    foreachRecordInModel(model, [&](QSqlRecord& record) {
        quint8 pos = record.value("order_num").toUInt();
        pos += direction ? -1: 1;
        record.setValue("order_num", pos);
    });
}

quint8 DatabaseManager::findMaxTaskPosInColumn(ColumnKey& columnKey)
{
    QSqlQuery query;
    query.prepare("SELECT MAX(order_num) AS max_order_num FROM column WHERE board_name = ? AND name = ?");
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
