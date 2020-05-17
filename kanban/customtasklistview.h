#ifndef CUSTOMTASKLISTVIEW_H
#define CUSTOMTASKLISTVIEW_H

#include <QListView>
#include <QMouseEvent>

class CustomTaskListView: public QListView
{
    Q_OBJECT

public:
    CustomTaskListView(QWidget* parent = nullptr);

    void mousePressEvent(QMouseEvent* event);

signals:
    void rightClicked(QModelIndex index, QPoint clickPos);
};

#endif // CUSTOMTASKLISTVIEW_H
