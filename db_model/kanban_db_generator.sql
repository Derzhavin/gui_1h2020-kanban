-- Creator:       MySQL Workbench 8.0.19/ExportSQLite Plugin 0.1.0
-- Author:        derzh
-- Caption:       New Model
-- Project:       Name of the project
-- Changed:       2020-05-11 18:37
-- Created:       2020-05-11 17:37
PRAGMA foreign_keys = OFF;

-- Schema: kanban
ATTACH "kanban.sdb" AS "kanban";
BEGIN;
CREATE TABLE "kanban"."user"(
  "name" VARCHAR(255) PRIMARY KEY NOT NULL,
  "password" VARCHAR(255) NOT NULL
);
CREATE TABLE "kanban"."board"(
  "name" VARCHAR(255) PRIMARY KEY NOT NULL,
  "description" TEXT,
  "user_name" VARCHAR(255) NOT NULL,
  "path_to_background" VARCHAR(300),
  CONSTRAINT "fk_board_user"
    FOREIGN KEY("user_name")
    REFERENCES "user"("name")
);
CREATE INDEX "kanban"."board.fk_board_user_idx" ON "board" ("user_name");
CREATE TABLE "kanban"."column"(
  "name" VARCHAR(255) PRIMARY KEY NOT NULL DEFAULT 'To do',
  "order_num" INTEGER NOT NULL CHECK("order_num">=0),
  "board_name" VARCHAR(255) NOT NULL,
  CONSTRAINT "fk_column_board1"
    FOREIGN KEY("board_name")
    REFERENCES "board"("name")
);
CREATE INDEX "kanban"."column.fk_column_board1_idx" ON "column" ("board_name");
CREATE TABLE "kanban"."task"(
  "deadline" DATETIME,
  "order_num" INTEGER NOT NULL CHECK("order_num">=0),
  "datetime_created" DATETIME NOT NULL,
  "column_name" VARCHAR(255) NOT NULL,
  "description" TEXT NOT NULL,
  CONSTRAINT "fk_task_column1"
    FOREIGN KEY("column_name")
    REFERENCES "column"("name")
);
CREATE INDEX "kanban"."task.fk_task_column1_idx" ON "task" ("column_name");
COMMIT;
