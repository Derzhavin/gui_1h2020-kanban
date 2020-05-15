#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ProjectWindow : public QMainWindow
{
    Q_OBJECT

public:
    ProjectWindow(QWidget *parent = nullptr);
    ~ProjectWindow();

public slots:
    void goToBoardsPushButtonClick();

signals:
    void reviewBoards();

private:
    Ui::MainWindow *ui;
};
#endif // PROJECTWINDOW_H
