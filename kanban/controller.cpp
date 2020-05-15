#include "controller.h"

Controller::Controller()
{    
    QObject::connect(&projectReviewDialog, SIGNAL(createBoard()), this, SLOT(createBoard()));
}

void Controller::run()
{
    projectReviewDialog.show();
}

void Controller::createBoard()
{
    qobject_cast<QDialog*>(sender())->close();
    createProjectDialog.show();
}
