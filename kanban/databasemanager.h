#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "singleton.h"
#include "schemacreatequery.h"

#include <QtSql/QSql>
#include <QtSql/QSqlQuery>
#include <QFile>
#include <QMessageBox>

class DatabaseManager: public Singleton<DatabaseManager>
{

public:
    DatabaseManager();

    QSqlDatabase database;
    QSqlQuery sqlQuery;
};

#endif // DATABASEMANAGER_H
