#ifndef SCHEMACREATEQUERY_H
#define SCHEMACREATEQUERY_H

#include <QString>
#include <QStringList>
#include <QtSql/QSqlQuery>

class SchemaCreateQuery
{
public:
    SchemaCreateQuery(QString& script);
    void exec();

private:
    QStringList statements;
};

#endif // SCHEMACREATEQUERY_H
