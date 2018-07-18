#include "manipulate_sqlite3.h"
#include "music.h"

#define MAXB 128
char sql_songs_index[MAXB] = "select filename from music";
char sql_song_path[MAXB] = "select absolute_path from music";

char* database1 = "./music.db";
char* database2 = "./ring.db";

int main(int argc, char *argv[])
{
	open_sqlite3(database2);
	show_interface();
    show_playlist(sql_songs_index);
	load_playlist(sql_song_path);
	menu();

//	printf("正在播放: 木小雅 - 可能否\n");
	//execl("/usr/bin/mplayer", "mplayer", " ", "/home/yang/music/songs/a.mp3", NULL);

	close_sqlite3();
	return 0;
}
