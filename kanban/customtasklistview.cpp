#include "customtasklistview.h"
#include <QDebug>

CustomTaskListView::CustomTaskListView(QWidget *parent): QListView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAcceptDrops(true);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDefaultDropAction(Qt::MoveAction);
}

void CustomTaskListView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());

    if (event->buttons() == Qt::RightButton) {

        if (index.isValid()) {
           selectionModel()->select(index, QItemSelectionModel::Select);
           QPoint clickPos = event->globalPos();
           emit rightClicked(index, clickPos);
        }
        selectionModel()->select(index, QItemSelectionModel::Deselect);
    }
    QListView::mousePressEvent(event);
}
