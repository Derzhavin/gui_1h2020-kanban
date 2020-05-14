#include "projectreviewdialog.h"
#include "ui_projectreviewdialog.h"

ProjectReviewDialog::ProjectReviewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectReviewDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->openBoardPushButton, SIGNAL(clicked()), this, SLOT(openBoardtPushButtonClick()));
    QObject::connect(ui->createBoardPushButton, SIGNAL(clicked()), this, SLOT(createBoardtPushButtonClick()));
}

void ProjectReviewDialog::createBoardtPushButtonClick()
{
    close();
    emit createBoard();
}

void ProjectReviewDialog::openBoardtPushButtonClick()
{
    close();
    emit openBoard();
}

ProjectReviewDialog::~ProjectReviewDialog()
{
    delete ui;
}
