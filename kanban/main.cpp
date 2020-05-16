#include "controller.h"

#include "taskinputdialog.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller controller;
    controller.run();

    return a.exec();
}
