#include "createprojectdialog.h"
#include "ui_createprojectdialog.h"

CreateProjectDialog::CreateProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateProjectDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->goBackPushButton, SIGNAL(clicked()), this, SLOT(goBackPushButtonClick()));
    QObject::connect(ui->CreateProjectPushButton, SIGNAL(clicked()), this, SLOT(createBoardPushButtonClick()));
}

CreateProjectDialog::~CreateProjectDialog()
{
    delete ui;
}

void CreateProjectDialog::clearEdits()
{
    ui->boardNameLineEdit->setText("");
    ui->descriptionTextEdit->setText("");
}

void CreateProjectDialog::goBackPushButtonClick()
{
    emit reviewBoardsClick();
}

void CreateProjectDialog::createBoardPushButtonClick()
{
    if (!ui->boardNameLineEdit->text().isEmpty()) {
        emit openBoardWindowClick();
    }
}
