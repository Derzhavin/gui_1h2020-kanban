#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "singleton.h"

#include <QtSql/QSql>
#include <QtSql/QSqlQuery>

class DatabaseManager: public Singleton<DatabaseManager>
{

public:
    DatabaseManager();
};

#endif // DATABASEMANAGER_H
