/*****************************************************************************
/*名称： music.h
/*描述： mplayer控制程序.c文件
/*作成日期：2018/07/03
/*参数：无
/*返回值：
/*作者：yang
/*******************************************************************************/
#include "music.h"
#include <string.h>
//some sql
char sql_select_songs[MAXB] = "select * from music where id = 1";
extern char* song_path[20];	
/*****************************************************************************
/*名称： show_playlist
/*描述： show play list
/*作成日期：2018/07/12
/*参数：  sql   sql_select_songname
/*返回值：
/*作者：yang
/*******************************************************************************/
void show_playlist(char* sql)
{
	printf("----------------------playlist-------------------------\n");
	printf("\n");
	
	exec_sqlite3(sql, load_song_name);
	
	printf("\n");
	printf("-------------------------------------------------------\n");
}
/*****************************************************************************
/*名称： show_interface
/*描述： show interface 
/*作成日期：2018/07/12
/*参数：无
/*返回值：
/*作者：yang
/*******************************************************************************/
void show_interface()
{
	CLEAR;
	PRINT_INTERFACE;
	SHOW;
}
/*****************************************************************************
/*名称： play_sequence
/*描述： play in order 
/*作成日期：2018/07/12
/*参数：sql   sql_select_path
/*返回值：
/*作者：yang
/*******************************************************************************/
void play_sequence(char* sql)
{
	int i;
	printf("play_sequence\n");
	exec_sqlite3(sql, load_song_path);
	
	for (i = 0; song_path[i] != 0; ++i)
	{
		// printf("last path: %s\n", song_path[i]);
		execl("/usr/bin/mplayer", "mplayer", " ", song_path[i], NULL);
		CLEAR;
	}
	// 	execl("/usr/bin/mplayer", "mplayer", " ", "/home/yang/music/songs/木小雅 - 可能否.mp3", NULL);
}

