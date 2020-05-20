-- Creator:       MySQL Workbench 8.0.19/ExportSQLite Plugin 0.1.0
-- Author:        derzh
-- Caption:       New Model
-- Project:       Name of the project
-- Changed:       2020-05-13 09:13
-- Created:       2020-05-11 17:37
PRAGMA foreign_keys = OFF;

-- Schema: kanban
ATTACH "kanban.sdb" AS "kanban";
BEGIN;
CREATE TABLE "kanban"."board"(
  "name" VARCHAR(255) PRIMARY KEY NOT NULL,
  "description" TEXT,
  "path_to_background" VARCHAR(300)
);
CREATE TABLE "kanban"."column"(
  "name" VARCHAR(255) NOT NULL DEFAULT 'To do',
  "order_num" INTEGER NOT NULL CHECK("order_num">=0),
  "board_name" VARCHAR(255) NOT NULL,
  PRIMARY KEY("board_name","name"),
  CONSTRAINT "fk_column_board1"
    FOREIGN KEY("board_name")
    REFERENCES "board"("name")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "kanban"."column.fk_column_board1_idx" ON "column" ("board_name");
CREATE TABLE "kanban"."task"(
  "deadline" DATETIME,
  "order_num" INTEGER NOT NULL CHECK("order_num">=0),
  "datetime_created" DATETIME NOT NULL,
  "description" TEXT NOT NULL,
  "board_name" VARCHAR(255) NOT NULL,
  "column_name" VARCHAR(255) NOT NULL,
  PRIMARY KEY("column_name","board_name","datetime_created"),
  CONSTRAINT "fk_task_column1"
    FOREIGN KEY("board_name","column_name")
    REFERENCES "column"("board_name","name")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "kanban"."task.fk_task_column1_idx" ON "task" ("board_name","column_name");
COMMIT;
