#ifndef CONFIG_H
#define CONFIG_H

#include <QtGlobal>

// common
using TaskUIntT = quint8;
using ColumnUIntT = quint8;
using BoardUintT = quint8;

//db
#define DB_CREATE_SCRIPT_PATH ":/resource/db_model/kanban_db_generator.sql"
#define DB_DEFAULT_PATH "kanban.sdb"
#define DB_FAIL_MSG "Application failed. Сontact developers to troubleshoot."
#define DB_MAX_BOARD_NAME 255
#define DB_MAX_BOARD_DESCRIPTION 65535
#define DB_MAX_COLUMN_NUM 255
#define DB_MAX_COLUMN_NAME 255
#define DB_MAX_TASK_NUM  255
#define DB_MAX_TASK_DESCRIPTION 65535
using RecordUIntT = quint8;

//ColumnWidget
#define COLUMN_WIDGET_WIDTH 300

#endif // CONFIG_H
