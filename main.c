#include "manipulate_sqlite3.h"
#include "music.h"
#define TETETTE \
	printf("+------------------------------+\n");\
	printf("|                              |\n");\
	printf("|  +-----------------------+   |\n");\
	printf("|  | +---------------------|   |\n");\
	printf("|  | |  |  +-------+  |   ||   |\n");\
	printf("|  | |  |  |       |  |   ||   |\n");\
	printf("|  | |  |  |       |  |   ||   |\n");\
	printf("|  | |  |  |       |  |   ||   |\n");\
	printf("|  | |  |  |       |  |   ||   |\n");\
	printf("|  | |  |  +-------+  |   ||   |\n");\
	printf("|  | |  +-------------+   ||   |\n");\
	printf("|  | +---------------------|   |\n");\
	printf("|  +-----------------------+   |\n");\
	printf("|                              |\n");\
	printf("+------------------------------+\n");

char sql_songs_index[MAXB] = "select filename from music";
char sql_song_path[MAXB] = "select absolute_path from music";

char* database1 = "./music.db";
char* database2 = "./ring.db";

int main(int argc, char *argv[])
{
	//PRINT_INTERFACE_NEW;
	init_pipe();
	TETETTE
	open_sqlite3(database1);
	//open_sqlite3(database2);
	//show_interface();
    //show_playlist(sql_songs_index);
		
	load_playlist(sql_song_path);
	//menu(); //进入主菜单

	close_sqlite3();
	
	return 0;
}
