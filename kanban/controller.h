#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "taskmanager.h"

#include "projectwindow.h"
#include "projectreviewdialog.h"
#include "createprojectdialog.h"
#include "boardselectiondialog.h"

#include "ui_projectreviewdialog.h"
#include "ui_projectwindow.h"
#include "ui_boardselectiondialog.h"
#include "ui_createprojectdialog.h"

#include <QGuiApplication>
#include <QScreen>
#include <QObject>
#include <QMessageBox>
#include <QInputDialog>

class Controller: public QObject
{
    Q_OBJECT
public:
    Controller();

    void run();
    void centerWidget(QWidget *widget);

public slots:
    void openBoard();
    void createBoard();
    void reviewBoards();
    void openBoardWindow();

    void addColumn();
    void addTask();

private:
    ProjectWindow projectWindow;
    ProjectReviewDialog projectReviewDialog;
    CreateProjectDialog createProjectDialog;
    BoardSelectionDialog boardSelectDialog;

    TaskManager taskManager;
//    BoardModel model;
};

#endif // CONTROLLER_H