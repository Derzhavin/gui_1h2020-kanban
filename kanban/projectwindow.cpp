#include "projectwindow.h"
#include "ui_projectwindow.h"

ProjectWindow::ProjectWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->goToBoardsPushButton, SIGNAL(clicked()), this, SLOT(goToBoardsPushButtonClicked()));
    QObject::connect(ui->newColumnToolButton, SIGNAL(clicked()), this, SLOT(addColumnToolButtonClicked()));
}

ProjectWindow::~ProjectWindow()
{
    delete ui;
}

void ProjectWindow::setBoardWithData(BoardLoad *boardLoad)
{
    ui->boardWidget->setData(boardLoad);
}

void ProjectWindow::goToBoardsPushButtonClicked()
{
    emit reviewBoardsClick();
}

void ProjectWindow::addColumnToolButtonClicked()
{
    emit addColumnClick();
}

void ProjectWindow::removeColumnPushButtonClicked()
{
    emit removeColumnClick(qobject_cast<ColumnWidget*>(sender()->parent()));
}

void ProjectWindow::renameColumnPushButtonClicked()
{
    emit renameColumnClick(qobject_cast<ColumnWidget*>(sender()->parent()));
}

void ProjectWindow::taskChosenClicked(ColumnWidget * columnWidgdet, QModelIndex &index, QPoint &pos)
{
    emit taskChosenClick(columnWidgdet, index, pos);
}

void ProjectWindow::addTaskPushButtonClicked()
{
    emit addTaskClick(qobject_cast<ColumnWidget*>(sender()->parent()));
}

void ProjectWindow::taskDraggedClicked(ColumnWidget *columnWidget, QModelIndex &index)
{
    emit taskDraggedClick(columnWidget, index);
}

bool ProjectWindow::taskIsDroppingClicked(ColumnWidget *columnWidget, QModelIndex &index)
{
    return emit taskIsDroppingClick(columnWidget, index);
}


void ProjectWindow::on_delBoardToolButton_clicked()
{
    emit removeBoardClick();
}
