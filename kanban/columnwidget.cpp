#include "columnwidget.h"
#include <QDebug>

ColumnWidget::ColumnWidget(QString columnName, QWidget *parent): QWidget(parent), columnName(columnName)
{
    setMaximumWidth(COLUMN_WIDGET_WIDTH);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

    columnNameLabel = new QLabel(columnName, this);
    removeColumnPushButton = new QPushButton("Remove column", this);
    renameColumnPushButton = new QPushButton("Rename column", this);
    addTaskPushButton = new QPushButton("Add Task");
    tasksListView = new CustomTaskListView(this);
    columnDataModel = new ColumnDataModel(this);

    tasksListView->setModel(columnDataModel);

    ProjectWindow* projectWindow = qobject_cast<ProjectWindow*>(QWidget::window());
    QObject::connect(removeColumnPushButton, SIGNAL(clicked()), projectWindow, SLOT(removeColumnPushButtonClick()));
    QObject::connect(renameColumnPushButton, SIGNAL(clicked()), projectWindow, SLOT(renameColumnPushButtonClick()));
    QObject::connect(addTaskPushButton, SIGNAL(clicked()), projectWindow, SLOT(addTaskPushButtonClick()));
    QObject::connect(tasksListView,
                     SIGNAL(taskChosen(ColumnWidget*, QModelIndex&, QPoint&)),
                     projectWindow,
                     SLOT(taskChosenClick(ColumnWidget*, QModelIndex&, QPoint&)));
    QObject::connect(tasksListView,
                     SIGNAL(taskDragged(ColumnWidget*, QModelIndex&)),
                     projectWindow,
                     SLOT(taskDraggedClick(ColumnWidget*, QModelIndex&)));
    QObject::connect(tasksListView,
                     SIGNAL(taskIsDropping(ColumnWidget*, QModelIndex&)),
                     projectWindow,
                     SLOT(taskIsDroppingClick(ColumnWidget*, QModelIndex&)));

    columnNameLabel->setAlignment(Qt::AlignCenter);

    setLayout(new QVBoxLayout);
    layout()->addWidget(columnNameLabel);
    layout()->addWidget(addTaskPushButton);
    layout()->addWidget(renameColumnPushButton);
    layout()->addWidget(removeColumnPushButton);
    layout()->addWidget(tasksListView);
}

void ColumnWidget::setColumnName(QString name)
{
    columnName = name;
    columnNameLabel->setText(name);
}

void ColumnWidget::pushFrontTask(QString &description, QString &datetimeCreated, QString &deadline)
{
    QString value = QString(description + "\n" +
                           "created at: " + datetimeCreated +
                           (deadline.isEmpty() ? "": "\ndeadline: " + deadline));

    if(columnDataModel->insertRow(columnDataModel->rowCount())) {
        QModelIndex index = columnDataModel->index(columnDataModel->rowCount() - 1, 0);
        columnDataModel->setData(index, value);
    }

    datetimeCreatedList.append(datetimeCreated);
}

void ColumnWidget::removeTask(QModelIndex &index)
{
    columnDataModel->removeRow(index.row());
    datetimeCreatedList.removeAt(index.row());
}

void ColumnWidget::updateTaskAt(QModelIndex& index, QString &description, QString &deadline)
{
    QString value = QString(description + "\n" +
                           "created at: " + datetimeCreatedList.at(index.row()) +
                           (deadline.isEmpty() ? "": "\ndeadline: " + deadline));

    columnDataModel->setData(index, value);
}

QString ColumnWidget::getTaskCreatedAt(TaskUIntT pos)
{
    return datetimeCreatedList.at(pos);
}

QString ColumnWidget::getColumnWidgetName()
{
    return columnName;
}
