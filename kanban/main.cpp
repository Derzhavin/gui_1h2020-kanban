#include "projectwindow.h"
#include "databasemanager.h"

#include <QApplication>
#include "projectreviewdialog.h"
#include "createprojectdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CreateProjectDialog w;
    w.show();
//    MainWindow w;
//    w.show();
    return a.exec();
}
