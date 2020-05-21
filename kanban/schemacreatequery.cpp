#include "schemacreatequery.h"

SchemaCreateQuery::SchemaCreateQuery(QString &script)
{
    int script_begin_pos = script.indexOf("PRAGMA");

    statements = script.mid(script_begin_pos, script.length()).split(';');
}

void SchemaCreateQuery::exec()
{
    foreach (QString statement, statements) {
        QSqlQuery query(statement);
        query.exec();
    }
}
