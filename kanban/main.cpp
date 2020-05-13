#include "mainwindow.h"
#include "taskmanager.h"

#include <QApplication>
#include <QDebug>
#include <tuple>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    TaskKey key = std::make_tuple(QString("a"), QString("k"), QString("2004-05-23 14:25:10"));

//    qDebug() << TaskManager::instance().getTask(key).value("board_name");
    MainWindow w;
    w.show();
    return a.exec();
}
