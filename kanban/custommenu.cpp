#include "custommenu.h"

CustomMenu::CustomMenu(QWidget *parent): QMenu(parent)
{

}

QVariant CustomMenu::getChosenAction()
{
    return action;
}

void CustomMenu::chosenAction(QAction *action)
{
    this->action = action ? QVariant(action->text()): QVariant();
}
