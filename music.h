#ifndef _MUSIC_H
#define _MUSIC_H
/*****************************************************************************
/*名称： music.h
/*描述： mplayer控制程序.c文件
/*作成日期：2018/07/03
/*参数：无
/*返回值：
/*作者：yang
/*******************************************************************************/

/* Include heads */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <termios.h>
#include <sys/stat.h>
#include "manipulate_sqlite3.h"  


enum PLAYER_OPTIONS
{
	QUIT = '0',
	PLAY,
	SWITCH_PREVIEW,
	PAUSE,
	SWITCH_NEXT,
	FAST_FORWARD,
	SINGLE_LOOP,
	LIST_LOOP,
	RANDOM,
	HELPME
};
// some defines
#define MAXB 128
#define FIFO "./cmdfifo"
#define MAXSONGNUM 6
#define CLEAR system("clear");
#define PRINT_INTERFACE \
		printf("---------------------------------------------------------\n");\
		printf("|       🎵 音 乐 播 放 器         Author: YANG          |\n");\
		printf("|                                              	        |\n");\
		printf("|                                          	        |\n");\
		printf("|       ▶ (1)   ⏪ (2)  ⏸  (3)   ⏩ (4)  ⏭  (5)         |\n");\
		printf("|                                         	        |\n");\
		printf("|       🔂 (6)  🔁 (7)  🔀 (8)   ❓ (9)  ❎ (0)         |\n");\
		printf("|                                              	        |\n");\
		printf("---------------------------------------------------------\n");
		
#define PRINT_INTERFACE_NEW \
		printf("---------------------------------------------------------\n");\
		printf("|       🎵 音 乐 播 放 器         Author: YANG          |\n");\
		printf("|                                              	        |\n");\
		printf("|                                          	        |\n");\
		printf("|       ▶  (1)    ⏸  (2)   ⏪ (3)   ⏩ (4)  ❎ (0)      |\n");\
		printf("|                                         	        |\n");\
		printf("|       ⏮  (5)    ⏭  (6)   🔂 (7)   🔁 (8)  🔀 (9)      |\n");\
		printf("|                                              	        |\n");\
		printf("---------------------------------------------------------\n");
//⏮🔇🔊🔉🔈❓ (h)
	 
#define SHOW \
		printf("\t1、播放	\t6、单曲循环\n");\
		printf("\t2、上一曲	7、列表循环\n");\
		printf("\t3、暂停	\t8、随机播放\n");\
		printf("\t4、下一曲	9、帮助\n");\
		printf("\t5、快进	\t0、退出\n");\
		printf("\n");

		
#define HELP \
		printf(----------------------------help------------------------------\n);\
		printf("\tb、\t播放列表 \n");\
		printf("\tg、\t歌曲名\n");\
		printf("\td、\t播放进度\n");\
		printf("\ts、\t时长\n");

/*****************************************************************************
/*名称： show_playlist
/*描述： show play list
/*作成日期：2018/07/12
/*参数：  sql   sql_select_songname
/*返回值：
/*作者：yang
/*******************************************************************************/
void show_playlist(char* sql);
/*****************************************************************************
/*名称： show_interface
/*描述： show interface 
/*作成日期：2018/07/12
/*参数：无
/*返回值：
/*作者：yang
/*******************************************************************************/
void init_pipe();
void show_interface();
void load_playlist(char* sql_path);
void do_play();
void do_exit();
void do_forward();
void do_rewind();
void do_preview();
void do_next();
void menu();
int kbhit(void);


#endif


