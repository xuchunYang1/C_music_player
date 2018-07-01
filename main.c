#include <stdio.h>
#include<unistd.h>
#include <sqlite3.h>

int main(int argc, char *argv[])
{
	
//	char sql_insert_table[128] = "insert into hello values(18, \"chuner\")";
	
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
	
#if 0
	//执行SQL
	ret = sqlite3_exec(db, sql_create_table, NULL, NULL, &errmsg);
	// if failed, record error log
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "create table fail: %s\n", errmsg);
	}
#endif
	sqlite3_close(db);
	printf("Close database\n");
	
	printf("正在播放: 木小雅 - 可能否\n");
	execl("/usr/bin/mplayer", "mplayer", " ", "/home/yang/music/songs/木小雅 - 可能否.mp3", NULL);

	return 0;
}
