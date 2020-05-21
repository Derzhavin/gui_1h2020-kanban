#ifndef UNFINISHEDKEEPER_H
#define UNFINISHEDKEEPER_H

#include "columnwidget.h"

typedef struct SavedDrag SavedDrag;
struct SavedDrag {
    ColumnWidget *columnWidget;
    QModelIndex index;
};

class UnfinishedKeeper
{
public:
    UnfinishedKeeper();

    void saveDrag(ColumnWidget *columnWidget, QModelIndex index);
    SavedDrag  getSavedDrag();

private:
    SavedDrag savedTask;
};

#endif // UNFINISHEDKEEPER_H
