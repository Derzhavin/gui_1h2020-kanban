#include "projectreviewdialog.h"
#include "ui_projectreviewdialog.h"

ProjectReviewDialog::ProjectReviewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectReviewDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->openBoardPushButton, SIGNAL(clicked()), this, SLOT(openBoardPushButtonClick()));
    QObject::connect(ui->createBoardPushButton, SIGNAL(clicked()), this, SLOT(createBoardPushButtonClick()));
}

void ProjectReviewDialog::createBoardPushButtonClick()
{
    emit createBoard();
}

void ProjectReviewDialog::openBoardPushButtonClick()
{
    emit openBoard();
}

ProjectReviewDialog::~ProjectReviewDialog()
{
    delete ui;
}
