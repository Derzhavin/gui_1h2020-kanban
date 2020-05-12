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
    query.prepare("INSERT INTO board (name, description, path_to_background) "
                    "VALUES (:name, :description, :path_to_background)");
    query.bindValue(":name", boardName);
    query.bindValue(":description", description ? *description: nullptr);
    query.bindValue(":path_to_background", pathToBackground ? *pathToBackground: nullptr);

    return query.exec() ? DatabaseQueryStatus::Success: DatabaseQueryStatus::Failure;
}
