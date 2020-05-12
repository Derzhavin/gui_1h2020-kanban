#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasemanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DatabaseManager::instance();
    DatabaseManager::instance();
    DatabaseManager::instance();
    DatabaseManager::instance();
    DatabaseManager::instance();
}

MainWindow::~MainWindow()
{
    delete ui;
}

