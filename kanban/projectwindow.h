#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include "columnwidget.h"

#include <functional>
#include <QMainWindow>
#include <QObject>

class ColumnWidget;

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
    void removeColumnPushButtonClick();
    void renameColumnPushButtonClick();

signals:
    void reviewBoards();
    void addColumn();
    void removeColumn(ColumnWidget *columnWidget);
    void renameColumn(ColumnWidget *columnWidget);

private:
    Ui::MainWindow *ui;
};
#endif // PROJECTWINDOW_H
