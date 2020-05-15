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

    QList<ColumnWidget*> columns;
};

#endif // BOARD_H
