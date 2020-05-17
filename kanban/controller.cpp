#include "controller.h"
#include <QDir>
#include <QDebug>

Controller::Controller()
{    
    QObject::connect(&projectReviewDialog, SIGNAL(createBoard()), this, SLOT(openBoard()));
    QObject::connect(&projectReviewDialog, SIGNAL(openBoard()), this, SLOT(createBoard()));

    QObject::connect(&createProjectDialog, SIGNAL(reviewBoards()), this, SLOT(reviewBoards()));
    QObject::connect(&createProjectDialog, SIGNAL(openBoardWindow()), this, SLOT(openBoardWindow()));

    QObject::connect(&boardSelectDialog, SIGNAL(reviewBoards()), this, SLOT(reviewBoards()));
    QObject::connect(&boardSelectDialog, SIGNAL(openBoardWindow()), this, SLOT(openBoardWindow()));

    QObject::connect(&projectWindow, SIGNAL(reviewBoards()), this, SLOT(reviewBoards()));
    QObject::connect(&projectWindow, SIGNAL(addColumn()), this, SLOT(addColumn()));
    QObject::connect(&projectWindow, SIGNAL(removeColumn(ColumnWidget*)), this, SLOT(removeColumn(ColumnWidget*)));
    QObject::connect(&projectWindow, SIGNAL(renameColumn(ColumnWidget*)), this, SLOT(renameColumn(ColumnWidget*)));
    QObject::connect(&projectWindow, SIGNAL(addTask(ColumnWidget*)), this, SLOT(addTask(ColumnWidget*)));
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

void Controller::openColumnNameInputDialog(std::function<void(QString& columnName)> callback)
{
    QString title = projectWindow.windowTitle();
    QString columnName = "";
    bool ok = true;

    while(ok and columnName.isEmpty()) {
        columnName = QInputDialog::getText(&projectWindow, title, "Enter the column name", QLineEdit::Normal, "", &ok);

        if (ok and !columnName.isEmpty()) {
            if (!taskManager.getColumn(columnName).data()) {
                callback(columnName);
            } else {
                QString msg =  "The column with this name is exist.";
                QMessageBox::information(&createProjectDialog,  createProjectDialog.windowTitle(), msg);
            }
        } else if (ok and columnName.isEmpty()){
            QString msg = "Column name must not be empty";
            QMessageBox::information(&createProjectDialog,  title, msg);
        }
    }
}

void Controller::openTaskInputDialog(std::function<void(QString &description, QString &deadline)> callback)
{
    taskInputdialog.exec();
    QString description = taskInputdialog.ui->descriptionTextEdit->toPlainText();
    QString deadline = taskInputdialog.ui->deadlineDateTimeEdit->text();
    callback(description, deadline);
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
            taskManager.currentBoardName = boardName;

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

void Controller::addColumn()
{
    openColumnNameInputDialog([&](QString &columnName) {
        BoardWidget *boardwidget = projectWindow.ui->boardWidget;
        ColumnWidget *columnWidget = new ColumnWidget(columnName, boardwidget);
        boardwidget->pushBackColumnWidget(columnWidget);

        taskManager.addColumn(columnName);
    });
}

void Controller::removeColumn(ColumnWidget *columnWidget)
{
    BoardWidget *boardWidget = projectWindow.ui->boardWidget;
    quint8 pos = boardWidget->getColumnWidgetPos(columnWidget) + 1;
    taskManager.removeColumn(columnWidget->columnName);
    boardWidget->removeColumnWidgetAtPos(pos - 1);
}

void Controller::renameColumn(ColumnWidget *columnWidget)
{
    openColumnNameInputDialog([&](QString &newColumnName) {
       taskManager.renameColumn(columnWidget->columnName, newColumnName);
       columnWidget->setColumnName(newColumnName);
    });
}

void Controller::addTask(ColumnWidget *columnWidget)
{
    openTaskInputDialog([&](QString &description, QString deadline) {
        QString columnName  = columnWidget->columnName;
        QString datetimeCreated = taskManager.addTask(columnName, description, deadline);
        columnWidget->pushFrontTask(description, datetimeCreated, deadline);
    });
}
