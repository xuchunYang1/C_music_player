#include "music.h"

#include <stdio.h>
#include <unistd.h>
#include <sqlite3.h>

int load_my_info(void* para, int n_column, char** column_value, char** column_name);

int load_my_info(void* para, int n_column, char** column_value, char** column_name)
{
	int i;
	printf("记录包含 %d 个字段\n", n_column);
	for (int i = 0; i < n_column; i++)
	{
		printf("字段名：%s  字段值：%s\n", column_name[i], column_value[i]);
	}
	
	printf("--------------\n");
	
	return 0;
}


int main(int argc, char *argv[])
{
	
//	char sql_insert_table[128] = "insert into hello values(18, \"chuner\")";
	char sql_select_songs[128] = "select * from file_path";
	//error
	char *errmsg = 0;
	
	//return value
	int ret = 0;
	
	//database point
	sqlite3 *db = 0;
	
	//open database
	ret = sqlite3_open("./music.db", &db);
	
	//if open failed
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "Cannot open db: %s\n", sqlite3_errmsg(db));
		return 1;
	}	
	//if open succeed
	printf("Open database\n");
	
	//执行SQL
		ret = sqlite3_exec(db, sql_select_songs, load_my_info, NULL, &errmsg);
		// if failed, record error log
		if (ret != SQLITE_OK)
		{
			fprintf(stderr, "create table fail: %s\n", errmsg);
		}
	
	sqlite3_close(db);
	printf("Close database\n");
	
	printf("正在播放: 木小雅 - 可能否\n");
	execl("/usr/bin/mplayer", "mplayer", " ", "/home/yang/music/songs/木小雅 - 可能否.mp3", NULL);

	return 0;
}
