#include "boardselectiondialog.h"
#include "ui_boardselectiondialog.h"

BoardSelectionDialog::BoardSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoardSelectionDialog)
{
    ui->setupUi(this);

    model = new QStringListModel;

    ui->boardsListView->setModel(model);
    ui->boardsListView->setSelectionMode(QAbstractItemView::SingleSelection);

    QObject::connect(ui->goBackPushButton, SIGNAL(clicked()), this, SLOT(goBackPushButtonClick()));
    QObject::connect(ui->boardsListView, SIGNAL(pressed(QModelIndex)), this, SLOT(listViewClick(QModelIndex)));
}

BoardSelectionDialog::~BoardSelectionDialog()
{
    delete ui;
}

void BoardSelectionDialog::setListViewWithData(BoardList *boardList)
{
    QStringList list;
    for(BoardUintT i = 0; i < boardList->size(); i++) {
        QString text = boardList->at(i);
        list.append(text);
    }
    model->setStringList(list);
}

QString BoardSelectionDialog::getSelectedBoard()
{
    return ui->chosenBoardLineEdit->text();
}

void BoardSelectionDialog::goBackPushButtonClick()
{
    emit reviewBoardsClick();
}

void BoardSelectionDialog::listViewClick(QModelIndex index)
{
    if (index.isValid()) {
        ui->chosenBoardLineEdit->setText(model->data(index).toString());
    }
}

void BoardSelectionDialog::on_applyChoicePushButton_clicked()
{
    if (ui->chosenBoardLineEdit->text().isEmpty()) {
        QMessageBox::information(this, this->windowTitle(), "No board was chosen.");
    } else {
        emit openExistingProjectWindowClick(ui->chosenBoardLineEdit->text());
    }
}
