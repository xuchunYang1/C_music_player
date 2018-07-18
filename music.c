/*****************************************************************************
/*名称： music.h
/*描述： mplayer控制程序.c文件
/*作成日期：2018/07/03
/*参数：无
/*返回值：
/*作者：yang
/*******************************************************************************/
#include "music.h"

//some sql
extern char* song_path[MAXSONGNUM];
//char play_flag = 1;
pid_t pid_play_g = 0; //播放进程的pid号
static int song_index_g = 0; //歌曲索引
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
	//SHOW;
}
/*****************************************************************************
/*名称： load_playlist
/*描述：  加载播放列表
/*作成日期：2018/07/12
/*参数：sql   sql_select_path
/*返回值：
/*作者：yang
/*******************************************************************************/
void load_playlist(char* sql_path)
{
	exec_sqlite3(sql_path, load_song_path); //song_path[] // 优化
}
/*****************************************************************************
/*名称： do_play
/*描述：  播放歌曲
/*作成日期：2018/07/17
/*参数：
/*返回值：
/*作者：yang
/*******************************************************************************/
void do_play()
{
	int status;
	pid_t ret;
	if (pid_play_g > 0)//pid must greater than 0 : fix system 
	{
		printf("kill pid %d \n",pid_play_g);
		kill(pid_play_g, 9);
	}

	pid_play_g = fork();
	if (pid_play_g < 0)
	{
		printf("fork error\n");
		exit(1);
	}
	else if (pid_play_g == 0) //子进程
	{
		printf("son process %s \n",song_path[song_index_g]);
		execl("/usr/bin/mplayer", "mplayer", " ", song_path[song_index_g], "-slave", "-quiet", NULL);
	}
	else //父进程
	{
		printf("parent process %s \n",song_path[song_index_g]);
		ret = wait(&status);
		if (ret < 0)
		{
			printf("wait error\n");
			exit(1);
		}

		if (WIFSIGNALED(status)) //手动切换,异常退出
		{
			song_index_g++;
			printf("下一首\n");
			exit(0);
		}
	}
}

/*名称： do_exit
/*描述：  播放歌曲
/*作成日期：2018/07/17
/*参数：
/*返回值：
/*作者：yang
/*******************************************************************************/
void do_exit()
{

}

/*名称： do_preview
/*描述：  播放歌曲
/*作成日期：2018/07/17
/*参数：
/*返回值：
/*作者：yang
/*******************************************************************************/
void do_preview()
{

}
/*****************************************************************************
/*名称： do_next
/*描述：  播放歌曲
/*作成日期：2018/07/17
/*参数：
/*返回值：
/*作者：yang
/*******************************************************************************/
void do_next()
{

}
/*****************************************************************************
/*名称： menu
/*描述： 播放菜单
/*作成日期：2015/4/30
/*参数：无
/*返回值：
/*作者：yang
/*******************************************************************************/
void menu()
{
	char ensure_exit = 0;
	char exit_flag = 1;
	char option = 'x';  //初始化为x,因为菜单里有0

	while (exit_flag)
	{
		show_interface();  //显示菜单界面
		while (kbhit() == 0);
		option = getchar();
		switch (option)
		{
			case QUIT:
						printf("确定退出吗？(y/n)\n"); //退出
					 	while (kbhit() == 0);
					 	ensure_exit = getchar();

					 	if (ensure_exit == 'y')
					 	{
					 		exit_flag = 0;
					 		// 杀死相关子进程
					 		printf("已退出, 欢迎使用!\n");
					 	}
			break;
			case PLAY: // 开始播放
				printf("PLAY!\n");
				do_play();
			break;
			case SWITCH_PREVIEW:  // 上一曲

			break;
			case PAUSE:  // 暂停

			break;
			case SWITCH_NEXT:  //下一曲

			break;
			case FAST_FORWARD:  //快进

			break;
			case SINGLE_LOOP: //单曲播放

			break;
			case LIST_LOOP: //循环播放

			break;
			case RANDOM:  //随机播放

			break;
			case HELPME: // 帮助 枚举不能与宏定义同名
			
			break;
			
		}
	}
}

/*************************************************
Function name: kbhit
Parameter    : void
Description	 : 获取键盘输入
Return		 : int，有输入返回1，否则返回0
Argument     : void
Autor & date :
**************************************************/
int kbhit(void)
{
  	struct termios oldt, newt;
  	int ch;
  	int oldf;
  	tcgetattr(STDIN_FILENO, &oldt);
  	newt = oldt;
  	newt.c_lflag &= ~(ICANON | ECHO);
  	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  	ch = getchar();
  	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  	fcntl(STDIN_FILENO, F_SETFL, oldf);
  	if(ch != EOF)
  	{
   	 	ungetc(ch, stdin);
    		return 1;
  	}
  	return 0;
}



