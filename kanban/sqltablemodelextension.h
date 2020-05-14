#ifndef SQLTABLEMODELEXTENSION_H
#define SQLTABLEMODELEXTENSION_H

#include <QtSql/QSqlTableModel>
#include <QSqlRecord>
#include <functional>

void foreachRecord(QSqlTableModel& model, std::function<void(QSqlRecord& record)> callback);

#endif // SQLTABLEMODELEXTENSION_H
