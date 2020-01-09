#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "proto.h"
#include "mysqlite.h"

static int sqlite_callback(void *para, int columnCount, char **columnValue, char **columnName);

/*
int main(int argc, char **argv)
{
    sqlite3 *db = NULL;
    char *zErrMsg = 0;
    int ret=0;
    char condition[64]={0};
    DEVICE_DATA device;

    //打开指定的数据库文件,如果不存在将创建一个同名的数据库文件
    ret = sqlite3_open("test.db", &db);
    if (ret)
    {
        fprintf(stderr, "Can't open database: %s/n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    else
        printf("open DB test ok \r\n");
    
    creat_table(db,"HAVC");

    sprintf(device.device_addr, "02#-101");
    sprintf(device.device_name, "HAVC");
    device.device_id=2101;
    device.temp=24;
    device.hum=66;
    device.dp_temp=18;
    device.dip_fire=0;
    device.PM25=12;
    device.smog=200;
    sprintf(condition, "hum=69");
    insert_data(db, NULL, &device);
    //查询数据   condition查询条件  []=[]
    select_data(db, device.device_name, condition, NULL);
    //修改数据库数据
    update(db, device.device_name, "hum=66", "temp", "39");
    //查询数据   condition查询条件  []=[]
    select_data(db, device.device_name, condition, NULL);
    //删除数据
    //del_data(db,device.device_name, "hum=24");
    
    //关闭数据库
    sqlite3_close(db); //关闭数据库
    return ret;
}
*/

//打开数据库
int open_db(char *path_name, sqlite3 **db)
{
    int ret = 0;
    //打开指定的数据库文件,如果不存在将创建一个同名的数据库文件
    ret = sqlite3_open("test.db", db);
    if (ret)
    {
        fprintf(stderr, "Can't open database: %s/n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        return ret;
    }
    else
        printf("open DB  ok \r\n");
    return 0;
}

//关闭数据库
int close_db(sqlite3 *db)
{
    return sqlite3_close(db);
}

//创建表
int creat_table(sqlite3 *db, char *table)
{
    int ret = 0;
    char sql[512] = {0};
    char *errmsg = NULL;

    sprintf(sql, "create table if not exists %s (\
    id integer primary key, device_id integer, \
    device_addr text, temp interger, hum interger, \
    dp_temp interger, dip_fire integer, smog integer,\
    PM25 integer);",
            table);
    ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);

    if (ret != SQLITE_OK)
    {
#ifdef DEBUG
        printf("create table error : %s\n", errmsg);
#endif
        return ret;
    }
#ifdef DEBUG
    printf("create table ok : %s\n", errmsg);
#endif

    return ret;
}

//表中插入数据
int insert_data(sqlite3 *db, char *table, const DEVICE_DATA *device)
{
    int ret = 0;
    char sql[512];
    char *errmsg = NULL;

    /*INSERT INTO TABLE_NAME [(column1, column2, column3,...columnN)]  
   VALUES (value1, value2, value3,...valueN);*/

    sprintf(sql, "INSERT INTO %s (device_id, device_addr, temp, hum, dp_temp, dip_fire, smog, PM25)\
    VALUES(%d, '%s', %d, %d, %d, %d, %d, %d)",
            device->device_name, device->device_id, device->device_addr, device->temp, device->hum, device->dp_temp, device->dip_fire, device->smog, device->PM25);

    ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);

#ifdef DEBUG
    printf("insert err:%s\r\n", errmsg);
#endif

    return ret;
}

//删除表中数据
int del_data(sqlite3 *db, char *table, char *condition)
{
    int ret = 0;
    char sql[128] = {0};
    char *errmsg = NULL;

    /*DELETE FROM COMPANY WHERE ID = 7;*/
    sprintf(sql, "DELETE FROM %s WHERE %s", table, condition);
    ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
#ifdef DEBUG
    printf("del err %s\r\n", errmsg);
#endif
    return ret;
}

//更新数据库数据   condition 更新条件    colum要更新的列        new_date  更新的数值
int update(sqlite3 *db, char *table, char *condition, char *column, char *new_date)
{
    int ret = 0;
    char sql[128] = {0};
    char *errmsg = NULL;
    //UPDATE 表 SET 列 = '新值' 【WHERE 条件语句】
    sprintf(sql, "UPDATE %s SET %s = %s WHERE %s", table, column, new_date, condition);
    ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);

#ifdef DEBUG
    printf("update err %s\r\n", errmsg);
#endif
    return ret;
}

//查询数据   condition查询条件  []=[]
int select_data(sqlite3 *db, char *table, char *condition, DEVICE_DATA *device_buf)
{
    int ret = 0;
    char sql[128] = {0};
    char *errmsg = NULL;
    
    SELECT_FLAG=1; 
    sprintf(sql, "SELECT *FROM %s WHERE %s", table, condition);
    ret = sqlite3_exec(db, sql, sqlite_callback, device_buf, &errmsg);
    SELECT_FLAG=0;

#ifdef DEBUG
    printf("select err %s\r\n", errmsg);
#endif

    return ret;
}

//回调函数
static int sqlite_callback(void *para, int columnCount, char **columnValue, char **columnName)
{
    int ret = 0;
    int i = 0;
    DEVICE_DATA_BUF *device_data_buf = (DEVICE_DATA_BUF *)para;
    static int num=0;
    
    //首次查询调用 回调函数
    if (SELECT_FLAG==0)
    {
        num=device_data_buf->device_num;
        SELECT_FLAG=1;
    }
    
    for (i = 0; i < columnCount; i++)
    {
        printf("callback--%s--%s\r\n", columnName[i], columnValue[i]);
        switch (i)
        {
        case DEVICE_ID:
            device_data_buf->device[num].device_id=atoi(columnValue[i]);
            break;
        case DEVICE_ADDR:
            memcpy(device_data_buf->device[num].device_addr, columnValue[i], NAME_SIZE);
            break;
        case TEMP:
            device_data_buf->device[num].temp=atoi(columnValue[i]);
            break;
        case HUM:
            device_data_buf->device[num].hum=atoi(columnValue[i]);
            break;
        case DP_TEMP:
            device_data_buf->device[num].dp_temp=atoi(columnValue[i]);
            break;
        case DIP_FIRE:
            device_data_buf->device[num].dip_fire=atoi(columnValue[i]);
            break;
        case SMOG:
            device_data_buf->device[num].smog=atoi(columnValue[i]);
            break;
        case PM25:
            device_data_buf->device[num].PM25=atoi(columnValue[i]);
        default:

            break;
        }
    }
    num++;

    return ret;
}
