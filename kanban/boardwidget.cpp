#include "boardwidget.h"

BoardWidget::BoardWidget(QWidget *parent): QWidget(parent)
{
    setLayout(new QHBoxLayout);
}

void BoardWidget::pushBackColumnWidget(ColumnWidget *column)
{
    column->setParent(this);
    layout()->addWidget(column);
    columns.push_back(column);
    column->show();
}
