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

    taskListModel = new QStringListModel();
    this->columnName = columnName;

    ProjectWindow* projectWindow = qobject_cast<ProjectWindow*>(QWidget::window());
    QObject::connect(removeColumnPushButton, SIGNAL(clicked()), projectWindow, SLOT(removeColumnPushButtonClick()));
    QObject::connect(renameColumnPushButton, SIGNAL(clicked()), projectWindow, SLOT(renameColumnPushButtonClick()));
    QObject::connect(addTaskPushButton, SIGNAL(clicked()), projectWindow, SLOT(addTaskPushButtonClick()));

    tasksListView->setDragEnabled(true);
    tasksListView->setAcceptDrops(true);
    tasksListView->setModel(taskListModel);

    columnNameLabel->setAlignment(Qt::AlignCenter);

    setLayout(new QVBoxLayout);
    layout()->addWidget(columnNameLabel);
    layout()->addWidget(addTaskPushButton);
    layout()->addWidget(renameColumnPushButton);
    layout()->addWidget(removeColumnPushButton);
    layout()->addWidget(tasksListView);

//    tasksListView->setDragEnabled(true);
//    tasksListView->setAcceptDrops(true);
//    QStringListModel *model = new QStringListModel();
//    tasksListView->setModel(model);
//    tasksListView->model()->insertRow(tasksListView->model()->rowCount());
//    QStringList list;
//    list << "11111111111111111111";
//    for(int i = 0; i < 20; i++){

//        list.append("2\n\1\1");
//    }
//    model->setStringList(list);
}

void ColumnWidget::setColumnName(QString name)
{
    columnName = name;
    columnNameLabel->setText(name);
}

void ColumnWidget::pushFrontTask(QString &description, QString &datetimeCreated, QString &deadline)
{
    QString task = QString(description + "\n" +
                           "created at: " + datetimeCreated +
                           (deadline.isEmpty() ? "": "\ndeadline: " + deadline));

    taskListModel->stringList().prepend(task);
}
