#ifndef _MANIPULATE_SQLITE3_H
#define _MANIPULATE_SQLITE3_H

/*****************************************************************************
/*名称： manipulate_sqlite3.h
/*描述： mplayer控制程序.c文件
/*作成日期：2018/07/03
/*参数：无
/*返回值：
/*作者：yang
/*******************************************************************************/

/* Include heads */
#include <stdio.h>
#include <sqlite3.h>
typedef int (*sqlite3_callback)(void*, int, char**, char**);



/*****************************************************************************
/*名称： open_sqlite3
/*描述： 打开数据库
/*作成日期：2018/07/03
/*参数：
		
/*返回值：
/*作者：yang
/*******************************************************************************/
int open_sqlite3();

/*****************************************************************************
/*名称： close_sqlite3
/*描述： 关闭数据库
/*作成日期：2018/07/03
/*参数：
		
/*返回值：
/*作者：yang
/*******************************************************************************/
void close_sqlite3();

/*****************************************************************************
/*名称： exec_sqlite3
/*描述： 执行sql
/*作成日期：2018/07/03
/*参数：
		
/*返回值：
/*作者：yang
/*******************************************************************************/
void exec_sqlite3(char *sql, sqlite3_callback callback);

/*****************************************************************************
/*名称： load_song_name
/*描述： 回调函数
/*作成日期：2018/07/04
/*参数：
		
/*返回值：
/*作者：yang
/*******************************************************************************/
int load_song_name(void* para, int n_col, char** col_value, char** col_name);

#endif
