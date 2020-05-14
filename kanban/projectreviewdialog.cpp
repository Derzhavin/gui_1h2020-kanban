#include "projectreviewdialog.h"
#include "ui_projectreviewdialog.h"

ProjectReviewDialog::ProjectReviewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectReviewDialog)
{
    ui->setupUi(this);
}

ProjectReviewDialog::~ProjectReviewDialog()
{
    delete ui;
}
