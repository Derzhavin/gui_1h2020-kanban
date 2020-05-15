#include "controller.h"

Controller::Controller()
{    
    QObject::connect(&projectReviewDialog, SIGNAL(createBoard()), this, SLOT(openBoard()));
    QObject::connect(&projectReviewDialog, SIGNAL(openBoard()), this, SLOT(createBoard()));

    QObject::connect(&createProjectDialog, SIGNAL(reviewBoards()), this, SLOT(reviewBoards()));
    QObject::connect(&createProjectDialog, SIGNAL(openBoardWindow()), this, SLOT(openBoardWindow()));

    QObject::connect(&boardSelectDialog, SIGNAL(reviewBoards()), this, SLOT(reviewBoards()));
    QObject::connect(&boardSelectDialog, SIGNAL(openBoardWindow()), this, SLOT(openBoardWindow()));

    QObject::connect(&projectWindow, SIGNAL(reviewBoards()), this, SLOT(reviewBoards()));
}

void Controller::run()
{
    projectReviewDialog.show();
}

void Controller::centerWidget(QWidget *widget)
{
    QRect r = widget->geometry();
    r.moveCenter(QGuiApplication::screens().first()->availableGeometry().center());
    widget->setGeometry(r);
}

void Controller::openBoard()
{
    qobject_cast<QDialog*>(sender())->close();
    createProjectDialog.show();
}

void Controller::createBoard()
{
    qobject_cast<QDialog*>(sender())->close();
    boardSelectDialog.show();
}

void Controller::reviewBoards()
{
    QWidget * widget = qobject_cast<QWidget*>(sender());

    if (!qobject_cast<ProjectWindow*>(widget)) {
        widget->close();
    }

    projectReviewDialog.show();
}

void Controller::openBoardWindow()
{
    QDialog *dialog = qobject_cast<QDialog*>(sender());

    if (qobject_cast<CreateProjectDialog*>(dialog)) {
        QString boardName = createProjectDialog.ui->boardNameLineEdit->text();
        QString description = createProjectDialog.ui->descriptionTextEdit->toPlainText();

        if (!taskManager.getBoard(boardName).data()) {
            taskManager.addBoard(boardName, description);

            createProjectDialog.clearEdits();
            createProjectDialog.close();

            centerWidget(&projectWindow);
            projectWindow.show();
        } else {
            QString msg =  "The board with this name is exist.";
            QMessageBox::information(&createProjectDialog,  createProjectDialog.windowTitle(), msg);
        }
    }
}
