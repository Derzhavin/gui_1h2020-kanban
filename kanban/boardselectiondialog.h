#ifndef BOARDSELECTIONDIALOG_H
#define BOARDSELECTIONDIALOG_H

#include <QDialog>

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

public slots:
    void goBackPushButtonClick();
    void applyChoicePushButtonClick();

signals:
    void reviewBoards();
    void openBoardWindow();

private:
    Ui::BoardSelectionDialog *ui;
};

#endif // BOARDSELECTIONDIALOG_H
