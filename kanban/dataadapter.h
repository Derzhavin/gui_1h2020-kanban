#ifndef DATAADAPTER_H
#define DATAADAPTER_H

#include <QList>
#include <QString>

struct Task {
    QString columnName;
    QString boardName;
    QString datetimeCreated;
    QString description;
    QString deadline;
    quint8 pos;
};

struct Column {
    QString columnName;
    QString boardName;
    quint8 pos;

    QList<Task> tasks;
};

using Board = QList<Column>;

#endif // DATAADAPTER_H
