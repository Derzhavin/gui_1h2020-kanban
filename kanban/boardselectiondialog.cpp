#include "boardselectiondialog.h"
#include "ui_boardselectiondialog.h"

BoardSelectionDialog::BoardSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoardSelectionDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->goBackPushButton, SIGNAL(clicked()), this, SLOT(goBackPushButtonClick()));
    QObject::connect(ui->applyChoicePushButton, SIGNAL(clicked()), this, SLOT(applyChoicePushButtonClick()));
}

BoardSelectionDialog::~BoardSelectionDialog()
{
    delete ui;
}

void BoardSelectionDialog::goBackPushButtonClick()
{
    emit reviewBoards();
}

void BoardSelectionDialog::applyChoicePushButtonClick()
{
    emit openBoardWindow();
}
