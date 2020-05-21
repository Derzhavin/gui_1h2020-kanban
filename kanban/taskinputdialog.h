#ifndef TASKINPUTDIALOG_H
#define TASKINPUTDIALOG_H

#include <QDialog>
#include <QObject>

namespace Ui {
class TaskInputDialog;
}

class TaskInputDialog : public QDialog
{
    Q_OBJECT

    friend class Controller;
public:
    explicit TaskInputDialog(QWidget *parent = nullptr);
    ~TaskInputDialog();

public slots:
    void on_checkBox_stateChanged(int arg1);

private:
    Ui::TaskInputDialog *ui;
};

#endif // TASKINPUTDIALOG_H
