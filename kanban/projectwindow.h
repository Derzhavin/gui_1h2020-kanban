#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include <functional>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ProjectWindow : public QMainWindow
{
    Q_OBJECT

    friend class Controller;
public:
    ProjectWindow(QWidget *parent = nullptr);
    ~ProjectWindow();

//    void show(std::function<void()> callback);

public slots:
    void goToBoardsPushButtonClick();
    void addColumnToolButtonClick();

signals:
    void reviewBoards();
    void addColumn();

private:
    Ui::MainWindow *ui;
};
#endif // PROJECTWINDOW_H
