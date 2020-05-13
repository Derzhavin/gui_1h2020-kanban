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
    QSqlQuery query;
    query.prepare("INSERT INTO board (name, description, path_to_background) VALUES (?, ?, ?)");
    query.addBindValue(boardName);
    query.addBindValue(description ? *description: nullptr);
    query.addBindValue(pathToBackground ? *pathToBackground: nullptr);

    return query.exec();
}

bool TaskManager::updateBoard(QString &boardName, QString* newBoardName, QString *newDescription, QString *newPathToBackground)
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

bool TaskManager::removeBoard(QString &boardName)
{
    QSqlQuery query;
    query.prepare("DELETE FROM board WHERE name = ?");
    query.addBindValue(boardName);

    return query.exec();
}

QSqlRecord TaskManager::getBoard(QString &boardName)
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

bool TaskManager::addColumn(ColumnKey &columnKey, quint8 &pos)
{
    QSqlTableModel model;
    selectColumns(model, columnKey, pos);
    shrinkModel(model, pos);

    QSqlRecord record;
    record.setValue("board_name", columnKey.first);
    record.setValue("name", columnKey.second);
    record.setValue("order_num", pos);
    model.insertRecord(0, record);

    return model.submitAll();
}

bool TaskManager::updateColumnPos(ColumnKey& columnKey, quint8& prevPos, quint8& newPos)
{
    QSqlTableModel model;
    selectColumns(model, columnKey, prevPos, newPos);
    shrinkModel(model, prevPos, newPos);

    quint8 recorInd = newPos > prevPos ? 0: model.rowCount();
    QSqlRecord record = model.record(recorInd);
    record.setValue("order_num", newPos);
    model.setRecord(recorInd, record);

    return model.submitAll();
}

bool TaskManager::updateColumnName(ColumnKey& columnKey, QString& newColumnName)
{
    QSqlQuery query;
    query.prepare("UPDATE column SET name = ? WHERE board_name = ? AND name = ?");
    query.addBindValue(newColumnName);
    bindColumnKey(query, columnKey);

    return query.exec();
}

bool TaskManager::removeColumn(ColumnKey &columnKey, quint8& prevPos)
{
    QSqlTableModel model;
    selectColumns(model, columnKey, prevPos);
    shrinkModel(model, prevPos);
    model.removeRows(0, 1);

    return model.submitAll();
}

bool TaskManager::addTask(TaskKey &taskKey, QString &description, quint8& pos, QString *deadline)
{
    QSqlTableModel model;
    selectTasks(model,taskKey, pos);
    shrinkModel(model, pos);

    QSqlRecord record;
    record.setValue("board_name", std::get<0>(taskKey));
    record.setValue("name", std::get<1>(taskKey));
    record.setValue("datetime_created", std::get<2>(taskKey));
    record.setValue("description", description);
    record.setValue("deadline", deadline ? *deadline: nullptr);
    model.insertRecord(0, record);

    return model.submitAll();
}

bool TaskManager::moveTaskToOtherColumn(TaskKey& taskKey, QString& columnName, quint8& prevPos, quint8& newPos)
{
    QSqlRecord record = getTask(taskKey);

    if (removeTask(taskKey, prevPos)) {
        taskKey = TaskKey(std::get<0>(taskKey), columnName, std::get<2>(taskKey));
        QString description = record.value("description").toString();
        QVariant variant = record.value("deadline");

        if (variant.isValid()) {
            QString description = variant.toString();
            return addTask(taskKey, description, newPos, &description);
        }

        return addTask(taskKey, description, newPos, nullptr);
    }
    return false;
}
bool TaskManager::updateTaskPosInColumn(TaskKey& taskKey, quint8& prevPos, quint8& newPos)
{
    QSqlTableModel model;
    selectTasks(model, taskKey, prevPos, newPos);
    shrinkModel(model, prevPos, newPos);

    quint8 recorInd = newPos > prevPos ? 0: model.rowCount();
    QSqlRecord record = model.record(recorInd);
    record.setValue("order_num", newPos);
    model.setRecord(recorInd, record);

    return model.submitAll();
}
bool TaskManager::updateTaskDescription(TaskKey& taskKey, QString& newDescription){
    QSqlQuery query;
    query.prepare("UPDATE task SET description = ? WHERE board_name = ? AND column_name = ? AND datetime_created = ?");
    query.addBindValue(newDescription);
    bindTaskKey(query, taskKey);

    return query.exec();
}

bool TaskManager::removeTask(TaskKey &taskKey, quint8& prevPos)
{
    QSqlTableModel model;
    selectTasks(model, taskKey, prevPos);
    shrinkModel(model, prevPos);
    model.removeRows(0, 1);

    return model.submitAll();
}

QSqlRecord TaskManager::getTask(TaskKey &taskKey)
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

void TaskManager::bindColumnKey(QSqlQuery &query, ColumnKey &columnKey)
{
    query.addBindValue(columnKey.first);
    query.addBindValue(columnKey.second);
}

void TaskManager::bindTaskKey(QSqlQuery &query, TaskKey &taskKey)
{
    query.addBindValue(std::get<0>(taskKey));
    query.addBindValue(std::get<1>(taskKey));
    query.addBindValue(std::get<2>(taskKey));
}

void TaskManager::selectTasks(QSqlTableModel &model, TaskKey& taskKey, quint8 begin, quint8 end)
{
    if (begin > end) {
        std::swap<quint8>(begin, end);
    }

    model.setTable("task");
    model.setSort(model.fieldIndex("order_num"), Qt::AscendingOrder);
    model.setFilter("board_name = " + std::get<0>(taskKey) +
                    " AND column_name = " + std::get<1>(taskKey) +
                    " AND datetime_created = " + std::get<2>(taskKey) +
                    " order_num BETWEEN " + QString::number(begin) + " AND " + QString::number(end));
    model.select();
}

void TaskManager::selectColumns(QSqlTableModel &model, ColumnKey &columnKey, quint8 begin, quint8 end)
{
    if (begin > end) {
        std::swap<quint8>(begin, end);
    }

    model.setTable("column");
    model.setSort(model.fieldIndex("order_num"), Qt::AscendingOrder);
    model.setFilter("board_name = " + columnKey.first + " AND name = " + columnKey.second +
                    " order_num BETWEEN " + QString::number(begin) + " AND " + QString::number(end));
    model.select();
}

void TaskManager::shrinkModel(QSqlTableModel &model, quint8 begin, quint8 end)
{
    foreachRecord(model, [&](QSqlRecord& record) {
        quint8 pos = record.value("order_num").toUInt();
        pos += end > begin ? -1: 1;
        record.setValue("order_num", pos);
    });
}
