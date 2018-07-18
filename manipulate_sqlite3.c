/*****************************************************************************
/*名称： manipulate_sqlite3.h
/*描述： mplayer控制程序.c文件
/*作成日期：2018/07/03
/*参数：无
/*返回值：
/*作者：yang
/*******************************************************************************/

#include "manipulate_sqlite3.h"
#include <string.h>
#include <stdlib.h>

static sqlite3 *db = 0; 	//database point
static int song_index = 0;  //歌曲的索引
static int path_index = 0;
char* song_path[MAXSONGNUM] = {NULL};  //save path
static int j = 0;	
/*****************************************************************************
/*名称： open_sqlite3
/*描述： 打开数据库
/*作成日期：2018/07/03
/*参数：
		
/*返回值：
/*作者：yang
/*******************************************************************************/
int open_sqlite3(char* database)
{	
	//return value
	int ret = 0;
	
	//open database
	ret = sqlite3_open(database, &db);
	
	//if open failed
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "Cannot open db: %s\n", sqlite3_errmsg(db));
		return 1;
	}	
	//if open succeed
	//printf("Open database\n");	
	return 0;
}

/*****************************************************************************
/*名称： close_sqlite3
/*描述： 关闭数据库
/*作成日期：2018/07/03
/*参数：
		
/*返回值：
/*作者：yang
/*******************************************************************************/
void close_sqlite3()
{
	sqlite3_close(db);
	
	//printf("Close database\n");
}

/*****************************************************************************
/*名称： exec_sqlite3
/*描述： 执行sql
/*作成日期：2018/07/03
/*参数：
		
/*返回值：
/*作者：yang
/*******************************************************************************/
void exec_sqlite3(char *sql, sqlite3_callback callback)
{
	//printf("exec_sqlite3\n");
	int ret = 0;
	//error
	char *errmsg = 0;
	//执行SQL
	ret = sqlite3_exec(db, sql, callback, NULL, &errmsg);
	// if failed, record error log
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "select fail: %s\n", errmsg);
	}
}

/*****************************************************************************
/*名称： load_song_name
/*描述： 回调函数
/*作成日期：2018/07/04
/*参数：
		
/*返回值：
/*作者：yang
/*******************************************************************************/
 int load_song_name(void* para, int n_col, char** col_value, char** col_name)
 {
 	//printf("load_song_name\n");
 	int i;
 	++song_index;
 	//printf("记录包含 %d 个字段\n", n_col);
 	for (i = 0; i < n_col; ++i)           
 	{
 		printf("%4d\t%s\n", song_index, col_value[i]);
 	}
	
 	return 0;
 }
/*****************************************************************************
/*名称： load_song_path
/*描述： 回调函数
/*作成日期：2018/07/04
/*参数：
		
/*返回值：
/*作者：yang
/*******************************************************************************/
int load_song_path(void* para, int n_col, char** col_value, char** col_name)
{
	//printf("load_song_path\n");
	int i;
	++path_index;

	for (i = 0; i < n_col; ++i)           
	{
		//赋值之前先给指针数组分配相应的空间,不然就出现段错误
		song_path[j] = malloc(strlen(col_value[i]) + 1);	
		if (song_path[j] == NULL)
		{
			printf("malloc failed\n");
			exit(1);
		}
		 
		strcpy(song_path[j], col_value[i]);
		//  //printf("%4d\t%s\n", path_index, col_value[i]);
		//printf("path: %s\n", song_path[j]);
		j++;
	}

	return 0;
}
