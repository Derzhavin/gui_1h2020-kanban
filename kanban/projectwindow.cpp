#include "projectwindow.h"
#include "ui_mainwindow.h"
#include "databasemanager.h"

ProjectWindow::ProjectWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

ProjectWindow::~ProjectWindow()
{
    delete ui;
}

