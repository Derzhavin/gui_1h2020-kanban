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
    if (!ui->chosenBoardLineEdit->text().isEmpty()) {
        emit openBoardWindow();
    } else {
        // To do:
        // Добавить анимацию chosenBoardLineEdit менеджером анимации
    }
}

////    QObject::connect(timeLine, &QTimeLine::finished, timeLine, &QTimeLine::deleteLater);
