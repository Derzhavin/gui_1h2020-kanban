#ifndef CREATEPROJECTDIALOG_H
#define CREATEPROJECTDIALOG_H

#include <QDialog>

namespace Ui {
class CreateProjectDialog;
}

class CreateProjectDialog : public QDialog
{
    Q_OBJECT

    friend class Controller;
public:
    explicit CreateProjectDialog(QWidget *parent = nullptr);
    ~CreateProjectDialog();

    void clearEdits();

public slots:
    void goBackPushButtonClick();
    void createBoardPushButtonClick();

signals:
    void reviewBoardsClick();
    void openBoardWindowClick();

private:
    Ui::CreateProjectDialog *ui;
};

#endif // CREATEPROJECTDIALOG_H
