#ifndef BOARDSELECTIONDIALOG_H
#define BOARDSELECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class BoardSelectionDialog;
}

class BoardSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BoardSelectionDialog(QWidget *parent = nullptr);
    ~BoardSelectionDialog();

private:
    Ui::BoardSelectionDialog *ui;
};

#endif // BOARDSELECTIONDIALOG_H
