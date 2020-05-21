#ifndef CUSTOMTASKLISTVIEW_H
#define CUSTOMTASKLISTVIEW_H

#include <QListView>
#include <QMouseEvent>

class ColumnWidget;

class CustomTaskListView: public QListView
{
    Q_OBJECT

public:
    CustomTaskListView(QWidget* parent = nullptr);

    void mousePressEvent(QMouseEvent* event);
    void dropEvent(QDropEvent *event);

signals:
    void taskChosen(ColumnWidget* columnWidget, QModelIndex& index, QPoint& clickPos);
    void taskDragged(ColumnWidget* columnWidget, QModelIndex& index);
    bool taskIsDropping(ColumnWidget* columnWidget, QModelIndex& index);
};

#endif // CUSTOMTASKLISTVIEW_H
