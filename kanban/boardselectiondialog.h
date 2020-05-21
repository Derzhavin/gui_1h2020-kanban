#ifndef BOARDSELECTIONDIALOG_H
#define BOARDSELECTIONDIALOG_H

#include "config.h"
#include "taskmanager.h"

#include <QMessageBox>
#include <QStringListModel>
#include <QDialog>
#include <QList>

namespace Ui {
class BoardSelectionDialog;
}

class BoardSelectionDialog : public QDialog
{
    Q_OBJECT

    friend class Controller;
public:
    explicit BoardSelectionDialog(QWidget *parent = nullptr);
    ~BoardSelectionDialog();

    void setListViewWithData(BoardList *boardList);
    QString getSelectedBoard();

public slots:
    void goBackPushButtonClick();
    void listViewClick(QModelIndex index);

signals:
    void reviewBoards();
    void openExistingProjectWindow(QString boardName);

private slots:
    void on_applyChoicePushButton_clicked();

private:
    Ui::BoardSelectionDialog *ui;
    QStringListModel *model;
};

#endif // BOARDSELECTIONDIALOG_H
