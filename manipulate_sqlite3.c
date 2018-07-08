/*****************************************************************************
/*名称： manipulate_sqlite3.h
/*描述： mplayer控制程序.c文件
/*作成日期：2018/07/03
/*参数：无
/*返回值：
/*作者：yang
/*******************************************************************************/

#include "manipulate_sqlite3.h"

sqlite3 *db = 0; 	//database point
static int song_index = 0;  //歌曲的索引

/*****************************************************************************
/*名称： open_sqlite3
/*描述： 打开数据库
/*作成日期：2018/07/03
/*参数：
		
/*返回值：
/*作者：yang
/*******************************************************************************/
int open_sqlite3()
{	
	//return value
	int ret = 0;
	
	//open database
	ret = sqlite3_open("./music.db", &db);
	
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
	int i;
	++song_index;
	//printf("记录包含 %d 个字段\n", n_col);
	for (int i = 0; i < n_col; i++)           
	{
		printf("%4d\t%s\n", song_index, col_value[i]);
	}
	
	return 0;
}
