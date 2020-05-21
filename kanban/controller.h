#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "taskmanager.h"

#include "projectwindow.h"
#include "projectreviewdialog.h"
#include "createprojectdialog.h"
#include "boardselectiondialog.h"
#include "taskinputdialog.h"
#include "boarddetailsdialog.h"

#include "ui_projectreviewdialog.h"
#include "ui_projectwindow.h"
#include "ui_boardselectiondialog.h"
#include "ui_createprojectdialog.h"
#include "ui_taskinputdialog.h"
#include "ui_boarddetailsdialog.h"

#include "custommenu.h"
#include "unfinishedkeeper.h"

#include <functional>
#include <QGuiApplication>
#include <QScreen>
#include <QObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QMenu>
#include <QDir>

class Controller: public QObject
{
    Q_OBJECT
public:
    Controller();

    void run();
    void centerWidget(QWidget *widget);

    void openColumnInputDialog(std::function<bool(QString& columnName)> callback);
    void openTaskInputDialog(std::function<bool(QString &, QString &)> callback);

public slots:
    void openBoard();
    void createBoard();
    void reviewBoards();
    void openBoardWindow();
    void openExistingProjectWindow(QString boardName);

    void showBoardDetails();
    void removeBoard();

    void addColumn();
    void removeColumn(ColumnWidget *columnWidget);
    void renameColumn(ColumnWidget *columnWidget);

    void addTask(ColumnWidget *columnWidget);
    void taskChosen(ColumnWidget* columnWidget, QModelIndex& index, QPoint &clickPos);
    void taskDragged(ColumnWidget* columnWidget, QModelIndex& index);
    bool taskIsDropping(ColumnWidget* columnWidgetTo, QModelIndex& indexTo);

private:
    ProjectWindow projectWindow;
    ProjectReviewDialog projectReviewDialog;
    CreateProjectDialog createProjectDialog;
    BoardSelectionDialog boardSelectDialog;
    TaskInputDialog taskInputDialog;
    BoardDetailsDialog boardDetailsDialog;

    TaskManager taskManager;
    UnfinishedKeeper unfinishedKeeper;
};

#endif // CONTROLLER_H
