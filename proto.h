#ifndef __QQ_PROTO_H
#define __QQ_PROTO_H

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sqlite3.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#pragma pack(1)    //指定字节对齐  

#define QQ_REG_SERVER_PORT	6666
#define QQ_SERVER_IP	"10.0.19.115"

#define NAME_SIZE   65
#define PWD_SIZE    33
#define CNT_SIZE    6
#define MAX_LEN    1024

#define DEBUG       1

typedef struct regdate
{
    char name[NAME_SIZE];
    char count[CNT_SIZE];
    char pwd[PWD_SIZE];
}reg_date_t;


typedef struct device_data
{
    char device_name[64];
    char device_addr[64];
    int device_id;
    uint8_t temp;
    uint8_t hum;
    int8_t dp_temp;        //露点温度
    uint8_t dip_fire;      //浸水—火光开关量  第0位 dip   第1位 fire    1 有信号   0无信号
    int smog;              //烟雾
    int PM25;
    
}DEVICE_DATA;

#endif

