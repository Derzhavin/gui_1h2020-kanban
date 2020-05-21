#include "columnwidget.h"

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

    ProjectWindow* projectWindow = qobject_cast<ProjectWindow*>(this->window());
    QObject::connect(removeColumnPushButton, SIGNAL(clicked()), projectWindow, SLOT(removeColumnPushButtonClicked()));
    QObject::connect(renameColumnPushButton, SIGNAL(clicked()), projectWindow, SLOT(renameColumnPushButtonClicked()));
    QObject::connect(addTaskPushButton, SIGNAL(clicked()), projectWindow, SLOT(addTaskPushButtonClicked()));
    QObject::connect(tasksListView,
                     SIGNAL(taskChosen(ColumnWidget*, QModelIndex&, QPoint&)),
                     projectWindow,
                     SLOT(taskChosenClicked(ColumnWidget*, QModelIndex&, QPoint&)));
    QObject::connect(tasksListView,
                     SIGNAL(taskDragged(ColumnWidget*, QModelIndex&)),
                     projectWindow,
                     SLOT(taskDraggedClicked(ColumnWidget*, QModelIndex&)));
    QObject::connect(tasksListView,
                     SIGNAL(taskIsDropping(ColumnWidget*, QModelIndex&)),
                     projectWindow,
                     SLOT(taskIsDroppingClicked(ColumnWidget*, QModelIndex&)));

    columnNameLabel->setAlignment(Qt::AlignCenter);

    setLayout(new QVBoxLayout);
    layout()->addWidget(columnNameLabel);
    layout()->addWidget(addTaskPushButton);
    layout()->addWidget(renameColumnPushButton);
    layout()->addWidget(removeColumnPushButton);
    layout()->addWidget(tasksListView);
}

ColumnWidget::ColumnWidget(QString columnName, Tasks tasks, QWidget *parent): ColumnWidget(columnName, parent)
{
    for(TaskUIntT i = 0; i < tasks.size(); i++) {
        TaskInfo taskInfo = tasks.at(i);
        pushFrontTask(taskInfo.description, taskInfo.datetimeCreated, taskInfo.deadline);
    }
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

QString ColumnWidget::getTaskDatetimeCreatedAt(TaskUIntT pos)
{
    return datetimeCreatedList.at(pos);
}

void ColumnWidget::removeTaskDatetimeCreatedAt(TaskUIntT pos)
{
    datetimeCreatedList.removeAt(pos);
}

void ColumnWidget::updatePosTaskDatetimeCreatedAt(TaskUIntT posFrom, TaskUIntT posTo)
{
    datetimeCreatedList.move(posFrom, posTo);
}

void ColumnWidget::addTaskDatetimeCreatedAt(TaskUIntT pos, QString datetimeCreated)
{
    datetimeCreatedList.insert(pos, datetimeCreated);
}

QString ColumnWidget::getColumnWidgetName()
{
    return columnName;
}
