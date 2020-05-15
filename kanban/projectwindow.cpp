#include "projectwindow.h"
#include "ui_projectwindow.h"

ProjectWindow::ProjectWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->goToBoardsPushButton, SIGNAL(clicked()), this, SLOT(goToBoardsPushButtonClick()));
}

ProjectWindow::~ProjectWindow()
{
    delete ui;
}

void ProjectWindow::show(std::function<void ()> callback)
{
    QWidget::show();
    callback();
}

void ProjectWindow::goToBoardsPushButtonClick()
{
    emit reviewBoards();
}

