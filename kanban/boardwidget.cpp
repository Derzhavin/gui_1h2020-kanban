#include "boardwidget.h"

BoardWidget::BoardWidget(QWidget *parent): QWidget(parent)
{
    setLayout(new QHBoxLayout);
}

void BoardWidget::pushBackColumnWidget(ColumnWidget *column)
{
    column->setParent(this);
    layout()->addWidget(column);
    columnWidgets.push_back(column);
    column->show();
}

void BoardWidget::removeColumnWidgetAtPos(ColumnUIntT pos)
{
    ColumnWidget *columnWidget = columnWidgets.at(pos);
    columnWidgets.removeAt(pos);
    delete columnWidget;
}

ColumnUIntT BoardWidget::getColumnWidgetPos(ColumnWidget *columnWidget)
{
    return columnWidgets.indexOf(columnWidget);
}

ColumnUIntT BoardWidget::columnsNum()
{
    return columnWidgets.length();
}
