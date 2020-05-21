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

void BoardWidget::setData(BoardLoad *boardLoad)
{
    for(ColumnUIntT i = 0; i < boardLoad->columns.size(); i++) {
        ColumnWidget *columnWidget = new ColumnWidget(boardLoad->columns.at(i).columnInfo.name,
                                                      boardLoad->columns.at(i).tasks,
                                                      this
                                                      );
        pushBackColumnWidget(columnWidget);
    }
}

ColumnUIntT BoardWidget::getColumnWidgetPos(ColumnWidget *columnWidget)
{
    return columnWidgets.indexOf(columnWidget);
}

ColumnUIntT BoardWidget::columnsNum()
{
    return columnWidgets.length();
}
