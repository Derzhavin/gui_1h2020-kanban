#include "customtasklistview.h"
#include "columnwidget.h"
#include <QDebug>

CustomTaskListView::CustomTaskListView(QWidget *parent): QListView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setDefaultDropAction(Qt::MoveAction);
    setAcceptDrops(true);
}

void CustomTaskListView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    ColumnWidget *columnWidget = qobject_cast<ColumnWidget*>(parent());

    if (event->buttons() == Qt::RightButton) {
        if (index.isValid()) {
           selectionModel()->select(index, QItemSelectionModel::Select);
           QPoint clickPos = event->globalPos();
           emit taskChosen(columnWidget, index, clickPos);
        }
        selectionModel()->select(index, QItemSelectionModel::Deselect);
    }
    else if (event->buttons() == Qt::LeftButton) {
//        qDebug() << "void CustomTaskListView::mousePressEvent(QMouseEvent *event) 28";
        emit taskDragged(columnWidget, index);
    }

    QListView::mousePressEvent(event);
}

void CustomTaskListView::dropEvent(QDropEvent *event)
{
    ColumnWidget *columnWidget = qobject_cast<ColumnWidget*>(parent());
    QModelIndex index  =indexAt(event->pos());

    if (emit taskIsDropping(columnWidget, index)) {
        qDebug() << "Drop event!";

        QListView::dropEvent(event);
    }
}
