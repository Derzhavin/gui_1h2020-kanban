#ifndef SQLPARSER_H
#define SQLPARSER_H

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

#endif // SQLPARSER_H
