#ifndef COLUMN_H
#define COLUMN_H

#include "config.h"

#include <QListView>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>

class ColumnWidget: public QWidget
{
public:
    ColumnWidget(QString columnName, QWidget *parent = nullptr);

    QPushButton *addTaskPushButton;
    QLineEdit *editColumnkLineEdit;
    QListView *tasksListView;
};

#endif // COLUMN_H
