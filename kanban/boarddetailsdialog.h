#ifndef BOARDDETAILSDIALOG_H
#define BOARDDETAILSDIALOG_H

#include <QDialog>
#include <QString>
#include <QMessageBox>

namespace Ui {
class BoardDetailsDialog;
}

class BoardDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BoardDetailsDialog(QWidget *parent = nullptr);
    ~BoardDetailsDialog();

    void setDescriptiinEdit(QString description);
    QString getDescriptionEditText();
    void setBoardNameEdit(QString boardName);
    QString getBoardNameEdit();

    bool isDescriptionEditChecked();
    bool isBoardnameEditChecked();

    bool isEditsClear();
    void clearEdits();

private slots:
    void boardNameCheckboxStateChanged(int arg1);
    void descriptionCheckboxStateChanged(int arg1);

private:
    Ui::BoardDetailsDialog *ui;
};

#endif // BOARDDETAILSDIALOG_H
