#include "columnwidget.h"

ColumnWidget::ColumnWidget(QString columnName, QWidget *parent): QWidget(parent)
{
    setMaximumWidth(COLUMN_WIDGET_WIDTH);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

    columnNameLabel = new QLabel(columnName, this);
    removeColumnPushButton = new QPushButton("Remove column", this);
    renameColumnPushButton = new QPushButton("Rename column", this);
    addTaskPushButton = new QPushButton("Add Task");
    tasksListView = new QListView(this);

    columnNameLabel->setAlignment(Qt::AlignCenter);

    setLayout(new QVBoxLayout);
    layout()->addWidget(columnNameLabel);
    layout()->addWidget(addTaskPushButton);
    layout()->addWidget(renameColumnPushButton);
    layout()->addWidget(removeColumnPushButton);
    layout()->addWidget(tasksListView);
}
