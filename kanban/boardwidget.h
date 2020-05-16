#ifndef BOARD_H
#define BOARD_H

#include "columnwidget.h"

#include <QList>
#include <QWidget>
#include <QHBoxLayout>

class BoardWidget: public QWidget
{
public:
    BoardWidget(QWidget *parent = nullptr);

    void pushBackColumnWidget(ColumnWidget *column);
    void removeColumnWidgetAtPos(quint8 pos);
    quint8 getColumnWidgetPos(ColumnWidget *columnWidget);
    quint8 columnsNum();

    QList<ColumnWidget*> columnWidgets;
};

#endif // BOARD_H
