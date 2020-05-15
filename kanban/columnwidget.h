#ifndef COLUMN_H
#define COLUMN_H

#include <QListView>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>

class ColumnWidget: public QWidget
{
public:
    ColumnWidget(QWidget *parent = nullptr);

    QPushButton *addTaskPushButton;
    QLineEdit *editTaskLineEdit;
    QListView *tasksListView;
};

#endif // COLUMN_H
