#include "custommenu.h"

CustomMenu::CustomMenu(QWidget *parent): QMenu(parent)
{
    QObject::connect(this, SIGNAL(triggered(QAction*)), this, SLOT(chosenAction(QAction*)));
}

QVariant CustomMenu::getChosenAction()
{
    return action;
}

void CustomMenu::chosenAction(QAction *action)
{
    this->action = action ? QVariant(action->text()): QVariant();
}
