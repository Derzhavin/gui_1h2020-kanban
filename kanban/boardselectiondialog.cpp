#include "boardselectiondialog.h"
#include "ui_boardselectiondialog.h"

BoardSelectionDialog::BoardSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoardSelectionDialog)
{
    ui->setupUi(this);
}

BoardSelectionDialog::~BoardSelectionDialog()
{
    delete ui;
}
