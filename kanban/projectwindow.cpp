#include "projectwindow.h"
#include "ui_projectwindow.h"

#include <QDebug>

ProjectWindow::ProjectWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->goToBoardsPushButton, SIGNAL(clicked()), this, SLOT(goToBoardsPushButtonClick()));
    QObject::connect(ui->newColumnToolButton, SIGNAL(clicked()), this, SLOT(addColumnToolButtonClick()));
}

ProjectWindow::~ProjectWindow()
{
    delete ui;
}

void ProjectWindow::goToBoardsPushButtonClick()
{
    emit reviewBoards();
}

void ProjectWindow::addColumnToolButtonClick()
{
    emit addColumn();
}

void ProjectWindow::removeColumnPushButtonClick()
{
    emit removeColumn(qobject_cast<ColumnWidget*>(sender()->parent()));
}

void ProjectWindow::renameColumnPushButtonClick()
{
    emit renameColumn(qobject_cast<ColumnWidget*>(sender()->parent()));
}

void ProjectWindow::addTaskPushButtonClick()
{
    emit addTask(qobject_cast<ColumnWidget*>(sender()->parent()));
}

void ProjectWindow::taskChosenClick(ColumnWidget *columnWidget, QModelIndex &index, QPoint& clickPos)
{
    emit taskChosen(columnWidget, index, clickPos);
}

