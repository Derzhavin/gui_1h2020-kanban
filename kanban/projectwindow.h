#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include "columnwidget.h"
#include "taskmanager.h"

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

    void setBoardWithData(BoardLoad *boardLoad);

public slots:
    void goToBoardsPushButtonClicked();

    void addColumnToolButtonClicked();
    void removeColumnPushButtonClicked();
    void renameColumnPushButtonClicked();

    void taskChosenClicked(ColumnWidget* columnWidgdet, QModelIndex& index, QPoint& pos);
    void addTaskPushButtonClicked();
    void taskDraggedClicked(ColumnWidget* columnWidget, QModelIndex& index);
    bool taskIsDroppingClicked(ColumnWidget* columnWidget, QModelIndex& index);

signals:
    void reviewBoardsClick();

    void removeBoardClick();

    void taskChosenClick(ColumnWidget*, QModelIndex&, QPoint&);
    void addColumnClick();
    void removeColumnClick(ColumnWidget *columnWidget);
    void renameColumnClick(ColumnWidget *columnWidget);

    void addTaskClick(ColumnWidget *columnWidget);
    void taskDraggedClick(ColumnWidget* columnWidget, QModelIndex& index);
    bool taskIsDroppingClick(ColumnWidget* columnWidget, QModelIndex& index);

private slots:
    void on_delBoardToolButton_clicked();

private:
    QMouseEvent *lastMouseEvent;
    Ui::MainWindow *ui;
};
#endif // PROJECTWINDOW_H
