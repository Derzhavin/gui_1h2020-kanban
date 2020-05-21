#include "boarddetailsdialog.h"
#include "ui_boarddetailsdialog.h"

BoardDetailsDialog::BoardDetailsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoardDetailsDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->boardNameCheckBox, SIGNAL(stateChanged(int)), this, SLOT(boardNameCheckboxStateChanged(int)));
    QObject::connect(ui->descriptionCheckBox, SIGNAL(stateChanged(int)), this, SLOT(descriptionCheckboxStateChanged(int)));
}

BoardDetailsDialog::~BoardDetailsDialog()
{
    delete ui;
}

void BoardDetailsDialog::setDescriptiinEdit(QString description)
{
    ui->descriptionTextEdit->setText(description);
}

QString BoardDetailsDialog::getDescriptionEditText()
{
    return ui->descriptionTextEdit->toPlainText();
}

void BoardDetailsDialog::setBoardNameEdit(QString boardName)
{
    ui->boardNameLineEdit->setText(boardName);
}

QString BoardDetailsDialog::getBoardNameEdit()
{
    return ui->boardNameLineEdit->text();
}

bool BoardDetailsDialog::isDescriptionEditChecked()
{
    return ui->descriptionCheckBox->isChecked();
}

bool BoardDetailsDialog::isBoardnameEditChecked()
{
    return ui->boardNameCheckBox->isChecked();
}

bool BoardDetailsDialog::isEditsClear()
{
    return ui->boardNameLineEdit->text().isEmpty() and ui->descriptionTextEdit->toPlainText().isEmpty();
}

void BoardDetailsDialog::clearEdits()
{
    ui->boardNameLineEdit->setText("");
    ui->descriptionTextEdit->setText("");
}

void BoardDetailsDialog::boardNameCheckboxStateChanged(int arg1)
{
    ui->boardNameLineEdit->setDisabled(arg1 == Qt::Checked ? false: true);
}

void BoardDetailsDialog::descriptionCheckboxStateChanged(int arg1)
{
    ui->descriptionTextEdit->setDisabled(arg1 == Qt::Checked ? false: true);
}
