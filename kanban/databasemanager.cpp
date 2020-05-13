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

DatabaseManager::DatabaseQueryStatus DatabaseManager::addBoard(QString &boardName, QString *description, QString *pathToBackground)
{
    QSqlQuery query;
    query.prepare("INSERT INTO board (name, description, path_to_background) VALUES (?, ?, ?)");
    query.addBindValue(boardName);
    query.addBindValue(description ? *description: nullptr);
    query.addBindValue(pathToBackground ? *pathToBackground: nullptr);

    return query.exec() ? DatabaseQueryStatus::Success: DatabaseQueryStatus::Failure;
}

DatabaseManager::DatabaseQueryStatus DatabaseManager::updateBoard(QString &boardName, QString* newBoardName, QString *newDescription, QString *newPathToBackground)
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

    return query.exec() ? DatabaseQueryStatus::Success: DatabaseQueryStatus::Failure;
}

DatabaseManager::DatabaseQueryStatus DatabaseManager::removeBoard(QString &boardName)
{
    QSqlQuery query;
    query.prepare("DELETE FROM board WHERE name = ?");
    query.addBindValue(boardName);

    return query.exec() ? DatabaseQueryStatus::Success: DatabaseQueryStatus::Failure;
}
