#include "manipulate_sqlite3.h"
#include "music.h"

char sql_songs_index[MAXB] = "select filename from music";
char sql_song_path[MAXB] = "select absolute_path from music";

int main(int argc, char *argv[])
{	
	open_sqlite3();
	// show_interface();
	// show_playlist(sql_songs_index);
	 play_sequence(sql_song_path);

//	printf("正在播放: 木小雅 - 可能否\n");
//	execl("/usr/bin/mplayer", "mplayer", " ", "/home/yang/music/songs/木小雅 - 可能否.mp3", NULL);
	
	close_sqlite3();
	return 0;
}
