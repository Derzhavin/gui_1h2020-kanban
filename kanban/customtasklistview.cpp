#include "customtasklistview.h"
#include <QDebug>

CustomTaskListView::CustomTaskListView(QWidget *parent): QListView(parent)
{

}

void CustomTaskListView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton) {
        QModelIndex index = indexAt(event->pos());
        QPoint clickPos = event->globalPos();
        emit rightClicked(index, clickPos);
    }
    else if (event->buttons() == Qt::LeftButton) {
        QListView::mousePressEvent(event);
    }
}
