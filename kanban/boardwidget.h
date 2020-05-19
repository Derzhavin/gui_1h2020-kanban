#ifndef BOARD_H
#define BOARD_H

#include "config.h"
#include "columnwidget.h"

#include <QList>
#include <QWidget>
#include <QHBoxLayout>

class BoardWidget: public QWidget
{
public:
    BoardWidget(QWidget *parent = nullptr);

    void pushBackColumnWidget(ColumnWidget *column);
    void removeColumnWidgetAtPos(ColumnUIntT pos);
    ColumnUIntT getColumnWidgetPos(ColumnWidget *columnWidget);
    ColumnUIntT columnsNum();

    QList<ColumnWidget*> columnWidgets;
};

#endif // BOARD_H
