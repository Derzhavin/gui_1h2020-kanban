#include "unfinishedkeeper.h"

UnfinishedKeeper::UnfinishedKeeper()
{

}

void UnfinishedKeeper::saveDrag(ColumnWidget *columnWidget, QModelIndex index)
{
    savedTask.columnWidget = columnWidget;
    savedTask.index = index;
}

SavedDrag UnfinishedKeeper::getSavedDrag()
{
    return savedTask;
}
