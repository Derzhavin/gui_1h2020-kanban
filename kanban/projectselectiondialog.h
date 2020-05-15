#ifndef PROJECTSELECTIONDIALOG_H
#define PROJECTSELECTIONDIALOG_H

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

#endif // PROJECTSELECTIONDIALOG_H
