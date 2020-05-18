#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "taskmanager.h"

#include "projectwindow.h"
#include "projectreviewdialog.h"
#include "createprojectdialog.h"
#include "boardselectiondialog.h"
#include "taskinputdialog.h"

#include "ui_projectreviewdialog.h"
#include "ui_projectwindow.h"
#include "ui_boardselectiondialog.h"
#include "ui_createprojectdialog.h"
#include "ui_taskinputdialog.h"

#include "custommenu.h"

#include <functional>
#include <QGuiApplication>
#include <QScreen>
#include <QObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QMenu>

class Controller: public QObject
{
    Q_OBJECT
public:
    Controller();

    void run();
    void centerWidget(QWidget *widget);

    void openColumnNameInputDialog(std::function<void(QString& columnName)> callback);
    void openTaskInputDialog(std::function<void(QString& description, QString& deadline)> callback);

public slots:
    void openBoard();
    void createBoard();
    void reviewBoards();
    void openBoardWindow();

    void addColumn();
    void removeColumn(ColumnWidget *columnWidget);
    void renameColumn(ColumnWidget *columnWidget);

    void addTask(ColumnWidget *columnWidget);
    void taskChosen(ColumnWidget*, QModelIndex&, QPoint &clickPos);

private:
    ProjectWindow projectWindow;
    ProjectReviewDialog projectReviewDialog;
    CreateProjectDialog createProjectDialog;
    BoardSelectionDialog boardSelectDialog;
    TaskInputDialog taskInputdialog;

    TaskManager taskManager;
//    BoardModel model;
};

#endif // CONTROLLER_H
