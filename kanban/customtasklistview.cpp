#include "customtasklistview.h"
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
    if (event->buttons() == Qt::RightButton) {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid()) {
           selectionModel()->select(index, QItemSelectionModel::Select);
           QPoint clickPos = event->globalPos();
           emit rightClicked(index, clickPos);
        }
        selectionModel()->select(index, QItemSelectionModel::Deselect);
    }
    QListView::mousePressEvent(event);
}
