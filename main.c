#include "manipulate_sqlite3.h"
#include "music.h"

#define MAXB 128

char sql_select_songs[MAXB] = "select * from music where id = 1";
char sql_songs_index[MAXB] = "select filename from music";

int main(int argc, char *argv[])
{	
	CLEAR;
	PRINT_INTERFACE;
	//SHOW;
	HELP;
	open_sqlite3();
	
	exec_sqlite3(sql_songs_index, load_song_name);
//	printf("正在播放: 木小雅 - 可能否\n");
//	execl("/usr/bin/mplayer", "mplayer", " ", "/home/yang/music/songs/木小雅 - 可能否.mp3", NULL);
	close_sqlite3();
	return 0;
}
