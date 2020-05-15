#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "projectwindow.h"
#include "projectreviewdialog.h"
#include "createprojectdialog.h"
#include "boardselectiondialog.h"

#include <QGuiApplication>
#include <QScreen>
#include <QObject>

class Controller: public QObject
{
    Q_OBJECT
public:
    Controller();

    void run();

public slots:
    void openBoard();
    void createBoard();

    void reviewBoards();
    void openBoardWindow();

private:
    ProjectWindow projectWindow;
    ProjectReviewDialog projectReviewDialog;
    CreateProjectDialog createProjectDialog;
    BoardSelectionDialog boardSelectDialog;
};

#endif // CONTROLLER_H
