#include "columnwidget.h"
#include <QDebug>

ColumnWidget::ColumnWidget(QString columnName, QWidget *parent): QWidget(parent)
{
    setMaximumWidth(COLUMN_WIDGET_WIDTH);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

    columnNameLabel = new QLabel(columnName, this);
    removeColumnPushButton = new QPushButton("Remove column", this);
    renameColumnPushButton = new QPushButton("Rename column", this);
    addTaskPushButton = new QPushButton("Add Task");
    tasksListView = new QListView(this);
    columnDataModel = new ColumnDataModel(tasksListView);

    this->columnName = columnName;

    ProjectWindow* projectWindow = qobject_cast<ProjectWindow*>(QWidget::window());
    QObject::connect(removeColumnPushButton, SIGNAL(clicked()), projectWindow, SLOT(removeColumnPushButtonClick()));
    QObject::connect(renameColumnPushButton, SIGNAL(clicked()), projectWindow, SLOT(renameColumnPushButtonClick()));
    QObject::connect(addTaskPushButton, SIGNAL(clicked()), projectWindow, SLOT(addTaskPushButtonClick()));

    QObject::connect(tasksListView, SIGNAL(clicked(QModelIndex)), this, SLOT(taskChosen(QModelIndex)));

    tasksListView->setDragEnabled(true);
    tasksListView->setAcceptDrops(true);
    tasksListView->setModel(columnDataModel);

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
    columnDataModel->addTask(description, datetimeCreated, deadline);
}

void ColumnWidget::taskChosen(QModelIndex index)
{

}
