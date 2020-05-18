#ifndef CUSTOMMENU_H
#define CUSTOMMENU_H

#include <QMenu>
#include <QString>
#include <QVariant>

class CustomMenu: public QMenu
{
    Q_OBJECT

public:
    CustomMenu(QWidget * parent = nullptr);

    QVariant getChosenAction();

public slots:
    void chosenAction(QAction *action);

private:
    QVariant action;
};

#endif // CUSTOMMENU_H
