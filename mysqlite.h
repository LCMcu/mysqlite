#ifndef __MYSQLITE_H
#define __MYSQLITE_H

#include <sqlite3.h>
#include "../myinclude/proto.h"

//打开数据库
int open_db(char *path_name, sqlite3 **db);

//创建表
int creat_table(sqlite3 *db, char *table);

//表中插入数据
int insert_data(sqlite3 *db, char *table, const DEVICE_DATA * device);

//查询数据   condition查询条件  []=[]
int select_data(sqlite3 *db, char *table, char *condition, DEVICE_DATA *device_buf);

//删除表中数据
int del_data(sqlite3 *db, char *table, char *condition);

//更新数据库数据   condition 更新条件    colum要更新的列        new_date  更新的数值
int update(sqlite3 *db, char *table, char *condition, char *column, char *new_date);

//关闭数据库
int close_db(sqlite3 *db);

#endif

