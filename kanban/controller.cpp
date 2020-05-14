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
    createProjectDialog.show();
}
