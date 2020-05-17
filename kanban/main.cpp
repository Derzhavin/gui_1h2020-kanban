#include "controller.h"

#include "databasemanager.h"

#include <QApplication>
#include <QDebug>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    Controller controller;
//    controller.run();
    QDateTime datetime = QDateTime::currentDateTime();
    QString datetimeCreated = datetime.toString("20-05-17 12:25::02");
    TaskKey key("b1", "c1", datetimeCreated);
    quint8 prevPos = 4;
    quint8 newPos = 5;
    QString description = "d3";
    QString newColumnName = "c2";
    qDebug() << DatabaseManager::instance().moveTaskToOtherColumn(key, newColumnName, newPos);
//    qDebug() << DatabaseManager::instance().deleteTask(key);
    return a.exec();
}
