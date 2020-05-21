#include "taskinputdialog.h"
#include "ui_taskinputdialog.h"

TaskInputDialog::TaskInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskInputDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->deadlineCheckBox, SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_stateChanged(int)));
}

TaskInputDialog::~TaskInputDialog()
{
    delete ui;
}

void TaskInputDialog::on_checkBox_stateChanged(int arg1)
{
    ui->deadlineDateTimeEdit->setDisabled(arg1 == Qt::Checked ? false: true);
}
