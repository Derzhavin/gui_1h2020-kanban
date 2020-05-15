#include "columnwidget.h"

ColumnWidget::ColumnWidget(QWidget *parent): QWidget(parent)
{
    setLayout(new QVBoxLayout);

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    addTaskPushButton = new QPushButton;
    editTaskLineEdit = new QLineEdit;
    hBoxLayout->addWidget(editTaskLineEdit);
    hBoxLayout->addWidget(addTaskPushButton);

    layout()->addItem(hBoxLayout);

    tasksListView = new QListView(this);
    layout()->addWidget(tasksListView);
}
