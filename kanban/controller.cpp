#include "controller.h"
#include <QDebug>

Controller::Controller()
{
    QObject::connect(&projectReviewDialog, SIGNAL(createBoardClick()), this, SLOT(createBoard()));
    QObject::connect(&projectReviewDialog, SIGNAL(openBoardClick()), this, SLOT(openBoard()));

    QObject::connect(&createProjectDialog, SIGNAL(reviewBoardsClick()), this, SLOT(reviewBoards()));
    QObject::connect(&createProjectDialog, SIGNAL(openBoardWindowClick()), this, SLOT(openBoardWindow()));

    QObject::connect(&boardSelectDialog, SIGNAL(reviewBoardsClick()), this, SLOT(reviewBoards()));
    QObject::connect(&boardSelectDialog,
                     SIGNAL(openExistingProjectWindowClick(QString)),
                     this,
                     SLOT(openExistingProjectWindow(QString)));

    QObject::connect(&projectWindow, SIGNAL(reviewBoardsClick()), this, SLOT(reviewBoards()));
    QObject::connect(&projectWindow, SIGNAL(addColumnClick()), this, SLOT(addColumn()));
    QObject::connect(&projectWindow, SIGNAL(removeColumnClick(ColumnWidget*)), this, SLOT(removeColumn(ColumnWidget*)));
    QObject::connect(&projectWindow, SIGNAL(renameColumnClick(ColumnWidget*)), this, SLOT(renameColumn(ColumnWidget*)));
    QObject::connect(&projectWindow, SIGNAL(addTaskClick(ColumnWidget*)), this, SLOT(addTask(ColumnWidget*)));
    QObject::connect(&projectWindow,
                     SIGNAL(taskChosenClick(ColumnWidget*, QModelIndex&, QPoint&)),
                     this,
                     SLOT(taskChosen(ColumnWidget*, QModelIndex&, QPoint&)));
    QObject::connect(&projectWindow,
                     SIGNAL(taskDraggedClick(ColumnWidget*, QModelIndex&)),
                     this,
                     SLOT(taskDragged(ColumnWidget*, QModelIndex&)));
    QObject::connect(&projectWindow,
                     SIGNAL(taskIsDroppingClick(ColumnWidget*, QModelIndex&)),
                     this,
                     SLOT(taskIsDropping(ColumnWidget*, QModelIndex&)));

    QObject::connect(&projectWindow, SIGNAL(removeBoardClick()), this, SLOT(removeBoard()));
    QObject::connect(&projectWindow, SIGNAL(showBoardDetailsClick()), this, SLOT(showBoardDetails()));
}

void Controller::run()
{
    projectReviewDialog.exec();
}

void Controller::centerWidget(QWidget *widget)
{
    QRect r = widget->geometry();
    r.moveCenter(QGuiApplication::screens().first()->availableGeometry().center());
    widget->setGeometry(r);
}

void Controller::openColumnInputDialog(std::function<bool(QString &)> callback)
{
    QString title = projectWindow.windowTitle();
    QString columnName = "";
    bool ok = true;
    bool callbackResult = false;

    while((ok and columnName.isEmpty()) and !callbackResult) {
        columnName = QInputDialog::getText(&projectWindow, title, "Enter the column name.", QLineEdit::Normal, "", &ok);

        if (ok and !columnName.isEmpty()) {
            if (!taskManager.getColumn(columnName).data()) {
                callbackResult = callback(columnName);
            } else {
                QString msg =  "The column with this name is exist.";
                QMessageBox::information(&createProjectDialog,  createProjectDialog.windowTitle(), msg);
            }
        }
        else if (ok and columnName.isEmpty()){
            QString msg = "Column name must not be empty.";
            QMessageBox::information(&createProjectDialog,  title, msg);
        }
    }
}

void Controller::openTaskInputDialog(std::function<bool(QString &description, QString &deadline)> callback)
{
    bool taskInputDialogResult = false;
    bool callbackResult = false;

    while(!taskInputDialogResult and !callbackResult) {
        taskInputDialog.exec();
        taskInputDialogResult = taskInputDialog.result();

        if (taskInputDialogResult) {
            QString description = taskInputDialog.ui->descriptionTextEdit->toPlainText();
            QString deadline = taskInputDialog.ui->deadlineDateTimeEdit->text();

            if (taskInputDialog.ui->deadlineCheckBox->checkState() == Qt::Unchecked) {
                deadline = "";
            }
            callbackResult = callback(description, deadline);
        }
    }
}

void Controller::openBoard()
{
    qobject_cast<QDialog*>(sender())->close();

    SharedPtrBoardList sharedPtrBoardList = taskManager.getBoards();
    if (sharedPtrBoardList.data()->size()) {
        boardSelectDialog.setListViewWithData(sharedPtrBoardList.data());
    }

    boardSelectDialog.show();
}

void Controller::createBoard()
{
    qobject_cast<QDialog*>(sender())->close();
    createProjectDialog.show();
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

        if (!taskManager.getBoardInfo(boardName).data()) {
            QString title = createProjectDialog.windowTitle();
            QString msg;

            TaskManager::OpStatus status = taskManager.addBoard(boardName, description);

            if (status == TaskManager::OpStatus::LongBoardName) {
                msg = "Board name is too long. Try again.";
            }
            else if (status == TaskManager::OpStatus::LongBoardDescription) {
                msg = "Board decription is too long. Try again.";
            }
            else if (status != TaskManager::OpStatus::Success) {
                msg = "Failed to add board. Try again.";
            }

            if (status != TaskManager::OpStatus::Success) {
                QMessageBox::information(&createProjectDialog,  title, msg);
            } else {
                taskManager.currentBoardName = boardName;

                createProjectDialog.clearEdits();
                createProjectDialog.close();

                centerWidget(&projectWindow);
                projectWindow.show();
            }

        } else {
            QString msg =  "The board with this name is exist.";
            QMessageBox::information(&createProjectDialog,  createProjectDialog.windowTitle(), msg);
        }
    }
}

void Controller::openExistingProjectWindow(QString boardName)
{
    qobject_cast<QDialog*>(sender())->close();

    taskManager.currentBoardName = boardName;
    QSharedPointer<BoardLoad> sharedPtrBoardLoad = taskManager.loadBoard();
    centerWidget(&projectWindow);
    projectWindow.setBoardWithData(sharedPtrBoardLoad.data());

    sharedPtrBoardLoad.clear();

    projectWindow.show();
}

void Controller::showBoardDetails()
{

    if (boardDetailsDialog.isEditsClear()) {
        QSharedPointer<BoardInfo> sharedPtrBoardInfo = taskManager.getBoardInfo();

        boardDetailsDialog.setBoardNameEdit(sharedPtrBoardInfo.data()->name);
        boardDetailsDialog.setDescriptiinEdit(sharedPtrBoardInfo.data()->description);
    }

    bool result = true, valid = false;
    while (result and !valid) {
        result = boardDetailsDialog.exec();

        if (result) {
            QString newBoardName;
            QString newDescription;

            if (boardDetailsDialog.isBoardnameEditChecked()) {
                newDescription = boardDetailsDialog.getDescriptionEditText();
            }
            if (boardDetailsDialog.isBoardnameEditChecked() and newBoardName != taskManager.currentBoardName) {
                newBoardName = boardDetailsDialog.getBoardNameEdit();
            }

            TaskManager::OpStatus status = taskManager.updateBoard(newBoardName, newDescription, "");
            QString title = createProjectDialog.windowTitle();
            QString msg;

            if (status == TaskManager::OpStatus::LongBoardName) {
                msg = "Board name is too long. Try again.";
            }
            else if (status == TaskManager::OpStatus::LongBoardDescription) {
                msg = "Board decription is too long. Try again.";
            }
            else if (status != TaskManager::OpStatus::Success) {
                msg = "Failed to update board details. Try again.";
            }
            else if (status == TaskManager::OpStatus::Success) {
                msg = "The board updated succesfully.";
            }

            if (status != TaskManager::OpStatus::Success) {
                QMessageBox::information(&createProjectDialog,  title, msg);
            } else {
                QMessageBox::information(&createProjectDialog,  title, msg);
                valid = true;
            }
        }
    }

}

void Controller::removeBoard()
{
    QMessageBox::StandardButton reply = QMessageBox::question(&projectWindow,
                                                              projectWindow.windowTitle(),
                                                              "Do you realy want to remove this board?");
    if (reply == QMessageBox::Yes) {
        TaskManager::OpStatus status = taskManager.removeBoard();

        if (status != TaskManager::OpStatus::Success) {
            QMessageBox::information(&projectWindow,  projectWindow.windowTitle(), "Failed to remove board. Try again.");
            return;
        }
        boardDetailsDialog.clearEdits();
        projectWindow.close();
        projectReviewDialog.show();
    }
}

void Controller::addColumn()
{
    QString title = projectWindow.windowTitle();
    QString msg;

    openColumnInputDialog([&](QString &columnName) {
        TaskManager::OpStatus status = taskManager.addColumn(columnName);

        if (status == TaskManager::OpStatus::LongColumnName) {
            msg = "Column name is too long. Try again.";
        }
        else if (status == TaskManager::OpStatus::TableFull) {
            msg = "Board is full of columns. You can't add any columns.";
        }
        else if (status != TaskManager::OpStatus::Success){
            msg = "Failed to add column. Try again.";
        }

        if (status != TaskManager::OpStatus::Success) {
            QMessageBox::information(&projectWindow,  title, msg);
            return false;
        }

        BoardWidget *boardwidget = projectWindow.ui->boardWidget;
        ColumnWidget *columnWidget = new ColumnWidget(columnName, boardwidget);
        boardwidget->pushBackColumnWidget(columnWidget);

        return true;
    });
}

void Controller::removeColumn(ColumnWidget *columnWidget)
{
    BoardWidget *boardWidget = projectWindow.ui->boardWidget;
    taskManager.removeColumn(columnWidget->getColumnWidgetName());
    quint8 pos = boardWidget->getColumnWidgetPos(columnWidget);
    boardWidget->removeColumnWidgetAtPos(pos);
}

void Controller::renameColumn(ColumnWidget *columnWidget)
{
    QString title = projectWindow.windowTitle();
    QString msg;

    openColumnInputDialog([&](QString &newColumnName) {
       TaskManager::OpStatus status = taskManager.renameColumn(columnWidget->getColumnWidgetName(), newColumnName);

       if (status == TaskManager::OpStatus::LongColumnName) {
           msg = "Column name is too long. Try again.";
       }
       else if (status != TaskManager::OpStatus::Success){
           msg = "Failed to rename column. Try again.";
       }

       if (status != TaskManager::OpStatus::Success){
            QMessageBox::information(&projectWindow,  title, msg);
            return false;
       }

       columnWidget->setColumnName(newColumnName);

       return true;
    });
}

void Controller::addTask(ColumnWidget *columnWidget)
{
    QString title = projectWindow.windowTitle();
    QString msg;

    openTaskInputDialog([&](QString &description, QString deadline) {
        QString columnName  = columnWidget->getColumnWidgetName();
        QString datetimeCreated;

        TaskManager::OpStatus status = taskManager.addTask(columnName, datetimeCreated, description, deadline);

        if (status == TaskManager::OpStatus::LongTaskDescription) {
            msg = "Task description is too long. Try again.";
        }
        else if (status == TaskManager::OpStatus::TableFull) {
            msg = "Column is full of tasks. You can't add tasks.";
        }
        else if (status != TaskManager::OpStatus::Success){
            msg = "Failed to add task. Try again.";
        }

        if (status != TaskManager::OpStatus::Success){
             QMessageBox::information(&projectWindow,  title, msg);
             return false;
        }

        columnWidget->pushFrontTask(description, datetimeCreated, deadline);

        return true;
    });
}

void Controller::taskChosen(ColumnWidget *columnWidget, QModelIndex &index,  QPoint& clickPos)
{
    CustomMenu menu(&projectWindow);

    menu.addAction("Update task");
    menu.addAction("Remove task");

    menu.exec(clickPos);

    QVariant variant = menu.getChosenAction();

    if (variant.isValid()) {
        QString choice = variant.toString();
        QString datetimeCreated = columnWidget->getTaskDatetimeCreatedAt(index.row());
        QString columnName = columnWidget->getColumnWidgetName();

        if (choice == "Remove task") {
            if (taskManager.removeTask(columnName, datetimeCreated) == TaskManager::OpStatus::Success) {
                columnWidget->removeTask(index);
            }
            else {
                QString title = projectWindow.windowTitle();
                QString msg = "Failed to remove task. Try again.";
                QMessageBox::information(&projectWindow,  title, msg);
            }
        }
        else if (choice == "Update task") {
            QString title = projectWindow.windowTitle();
            QString msg;

            openTaskInputDialog([&] (QString &description, QString& deadline) {

                TaskManager::OpStatus status = taskManager.updateTask(columnName, datetimeCreated, description, deadline);

                if (status == TaskManager::OpStatus::LongTaskDescription) {
                    msg = "Task description is too long. Try again.";
                }
                else if (status != TaskManager::OpStatus::Success){
                    msg = "Failed to rename task. Try again.";
                }

                if (status != TaskManager::OpStatus::Success){
                     QMessageBox::information(&projectWindow,  title, msg);
                     return false;
                }

                columnWidget->updateTaskAt(index, description, deadline);

                return true;
            });
        }
    }
}

void Controller::taskDragged(ColumnWidget *columnWidget, QModelIndex &index)
{
    unfinishedKeeper.saveDrag(columnWidget, index);
}

bool Controller::taskIsDropping(ColumnWidget *columnWidgetTo, QModelIndex &indexTo)
{
    SavedDrag savedDrag = unfinishedKeeper.getSavedDrag();
    QModelIndex indexFrom = savedDrag.index;
    ColumnWidget *columnWidgetFrom = savedDrag.columnWidget;

    QString columNameFrom = columnWidgetFrom->getColumnWidgetName();
    QString columnnameTo = columnWidgetTo->getColumnWidgetName();
    QString datetimeCreated = savedDrag.columnWidget->getTaskDatetimeCreatedAt(indexFrom.row());
    TaskUIntT newPos = indexTo.row() == -1 ? 0: indexTo.row() + 1;
    QString title = projectWindow.windowTitle();
    QString msg;

    if (columnWidgetTo == columnWidgetFrom) {
        if (taskManager.updateTaskPosInColumn(columNameFrom, datetimeCreated, newPos) == TaskManager::OpStatus::Failure) {
            msg = "Failed to move task. Try again.";
            QMessageBox::information(&projectWindow,  title, msg);

            return false;
        }
        columnWidgetTo->updatePosTaskDatetimeCreatedAt(indexFrom.row(), indexTo.row());
        return true;
    }

    TaskManager::OpStatus status = taskManager.moveTaskToOtherColumn(columNameFrom, datetimeCreated, columnnameTo, newPos);

    if (status == TaskManager::OpStatus::TableFull) {
        msg = "Column you tried to drop task is full. You can't add task at it.";
    }
    else if (status != TaskManager::OpStatus::Success) {
        msg = "Failed to move task. Try again.";
    }

    if (status != TaskManager::OpStatus::Success) {
        QMessageBox::information(&projectWindow,  title, msg);

        return false;
    }
    columnWidgetFrom->removeTaskDatetimeCreatedAt(indexFrom.row());
    columnWidgetTo->addTaskDatetimeCreatedAt(indexTo.row(), datetimeCreated);

    return true;
}
