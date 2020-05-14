#ifndef PROJECTREVIEWDIALOG_H
#define PROJECTREVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class ProjectReviewDialog;
}

class ProjectReviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectReviewDialog(QWidget *parent = nullptr);
    ~ProjectReviewDialog();

private:
    Ui::ProjectReviewDialog *ui;
};

#endif // PROJECTREVIEWDIALOG_H
