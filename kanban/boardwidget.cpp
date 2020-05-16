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

void BoardWidget::removeColumnWidgetAtPos(quint8 pos)
{
    ColumnWidget *columnWidget = columnWidgets.at(pos);
    columnWidgets.removeAt(pos);
    delete columnWidget;
}

quint8 BoardWidget::getColumnWidgetPos(ColumnWidget *columnWidget)
{
    return columnWidgets.indexOf(columnWidget);
}

quint8 BoardWidget::columnsNum()
{
    return columnWidgets.length();
}
