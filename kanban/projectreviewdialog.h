#ifndef PROJECTREVIEWDIALOG_H
#define PROJECTREVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class ProjectReviewDialog;
}

class ProjectReviewDialog : public QDialog
{
    Q_OBJECT

    friend class Controller;
public:
    explicit ProjectReviewDialog(QWidget *parent = nullptr);
    ~ProjectReviewDialog();

public slots:
    void createBoardPushButtonClick();
    void openBoardPushButtonClick();

signals:
    void createBoardClick();
    void openBoardClick();

private:
    Ui::ProjectReviewDialog *ui;
};

#endif // PROJECTREVIEWDIALOG_H
