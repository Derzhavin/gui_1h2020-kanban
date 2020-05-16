#include "columnwidget.h"

ColumnWidget::ColumnWidget(QString columnName, QWidget *parent): QWidget(parent)
{
    setMaximumWidth(COLUMN_WIDGET_WIDTH);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

    addTaskPushButton = new QPushButton;
    editColumnkLineEdit = new QLineEdit;
    editColumnkLineEdit->setText(columnName);

    setLayout(new QVBoxLayout);
    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(editColumnkLineEdit);
    hBoxLayout->addWidget(addTaskPushButton);

    layout()->addItem(hBoxLayout);

    tasksListView = new QListView(this);
    layout()->addWidget(tasksListView);
}
