#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include "columnwidget.h"

#include <functional>
#include <QMainWindow>
#include <QObject>

class ColumnWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ProjectWindow : public QMainWindow
{
    Q_OBJECT

    friend class Controller;
public:
    ProjectWindow(QWidget *parent = nullptr);
    ~ProjectWindow();

public slots:
    void goToBoardsPushButtonClick();

    void addColumnToolButtonClick();
    void removeColumnPushButtonClick();
    void renameColumnPushButtonClick();

    void addTaskPushButtonClick();
    void taskChosenClick(ColumnWidget *columnWidget, QModelIndex &index, QPoint& clickPos);
    void taskDraggedClick(ColumnWidget* columnWidget, QModelIndex& index);
    bool taskIsDroppingClick(ColumnWidget* columnWidget, QModelIndex& index);

signals:
    void reviewBoards();

    void addColumn();
    void removeColumn(ColumnWidget *columnWidget);
    void renameColumn(ColumnWidget *columnWidget);

    void addTask(ColumnWidget *columnWidget);
    void taskChosen(ColumnWidget *columnWidget, QModelIndex &index, QPoint& clickPos);
    void taskDragged(ColumnWidget* columnWidget, QModelIndex& index);
    bool taskIsDropping(ColumnWidget* columnWidget, QModelIndex& index);

private:
    QMouseEvent *lastMouseEvent;
    Ui::MainWindow *ui;
};
#endif // PROJECTWINDOW_H
