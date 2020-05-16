#ifndef COLUMN_H
#define COLUMN_H

#include "config.h"
#include "projectwindow.h"

#include <QListView>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QObject>

class ColumnWidget: public QWidget
{
    Q_OBJECT

public:
    ColumnWidget(QString columnName, QWidget *parent = nullptr);
    ~ColumnWidget();

    QPushButton *addTaskPushButton;
    QPushButton *removeColumnPushButton;
    QPushButton *renameColumnPushButton;
    QLabel *columnNameLabel;
    QListView *tasksListView;

    QString columnName;
};

#endif // COLUMN_H
