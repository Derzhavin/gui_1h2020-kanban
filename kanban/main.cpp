#include "mainwindow.h"
#include "databasemanager.h"

#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString k("k");
    QString c("c");
    QString y("y");
    QString f("f");

    DatabaseManager::instance().updateBoard(f, &y, nullptr, &k);

    MainWindow w;
    w.show();
    return a.exec();
}
