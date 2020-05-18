#include "controller.h"
#include "columnwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller controller;
    controller.run();

    return a.exec();
}
