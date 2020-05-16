#include "controller.h"

#include "taskinputdialog.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

       TaskInputDialog d;
       d.exec();
//    Controller controller;
//    controller.run();
    return a.exec();
}
