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
int pipe_fd = -1;  //管道描述符

char fast_forward[] = "seek 10";
char fast_backward[] = "seek -10";
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
/*名称： init_pipe
/*描述： show play list
/*作成日期：2018/07/12
/*参数：  sql   sql_select_songname
/*返回值：
/*作者：yang
/*******************************************************************************/
void init_pipe()
{
	int res;
	if (access("./cmdfifo", F_OK) == 0)
	{
		//管道文件存在,删除重建
		unlink(FIFO);
		res = mkfifo("./cmdfifo", 0777);
		if (res != 0)
		{
			fprintf(stderr, "Could not create fifo %s\n", "./cmdfifo");
			exit(1);
		}
		printf("fifo success res = %d\n", res);
	}
	else
	{
		res = mkfifo("./cmdfifo", 0777);
		if (res != 0)
		{
			fprintf(stderr, "Could not create fifo %s\n", "./cmdfifo");
			exit(1);
		}
	}
	
	pipe_fd = open("./cmdfifo", O_WRONLY|O_NONBLOCK); //只写非阻塞打开管道|O_NONBLOCK)
	printf("pipe_fd = %d\n", pipe_fd);
	if (pipe_fd < 0)
	{
		printf("open error\n");
		exit(1);
	}
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
	else if (pid_play_g == 0) //子进程  "-idle"
	{
		execl("/usr/bin/mplayer", "mplayer", "-slave", "-quiet", "-input", "file=./cmdfifo",  song_path[song_index_g], NULL);
		show_interface();
		printf("son process %s \n",song_path[song_index_g]);
	}
	else //父进程
	{
		show_interface();
		printf("parent process %s \n",song_path[song_index_g]);
#if 0
		printf("parent process %s \n",song_path[song_index_g]);
		ret = wait(&status);
		if (ret < 0)
		{
			printf("wait error\n");
			exit(1);
		}

		if (WIFSIGNALED(status)) //手动切换,异常退出
		{
			//song_index_g++;
			printf("下一首\n");
			exit(0);
		}
#endif
	}
}
/*******************************************************************************/
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
/*******************************************************************************/
/*名称： do_preview
/*描述：  播放歌曲
/*作成日期：2018/07/17
/*参数：
/*返回值：
/*作者：yang
/*******************************************************************************/
void do_preview()
{
	--song_index_g;
	
	if (song_index_g < 0)
	{
		song_index_g = MAXSONGNUM - 1;
	}
	
	do_play();
	
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
	++song_index_g;
	if (song_index_g >= MAXSONGNUM)
	{
		song_index_g = 0;
	}

	do_play();
}
/*****************************************************************************
/*名称： do_forward
/*描述：  播放歌曲
/*作成日期：2018/07/17
/*参数：
/*返回值：
/*作者：yang
/*******************************************************************************/
void do_forward()
{	
	write(pipe_fd, "seek 10", sizeof(fast_forward));
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
					 		kill(pid_play_g, 9);
					 		printf("已退出, 欢迎使用!\n");
					 	}
			break;
			case PLAY: // 开始播放
				printf("PLAY!\n");
				do_play();
			break;
			case SWITCH_PREVIEW:  // 上一曲
				printf("PREVIEW!\n");
				do_preview();
			break;
			case PAUSE:  // 暂停

			break;
			case SWITCH_NEXT:  //下一曲
				printf("NEXT!\n");
				do_next();
			break;
			case FAST_FORWARD:  //快进
				printf("FAST_FORWARD\n");
				do_forward();
				
			break;
			case SINGLE_LOOP: //单曲播放

			break;
			case LIST_LOOP: //循环播放

			break;
			case RANDOM:  //随机播放

			break;
			case HELPME: // 帮助 枚举不能与宏定义同名
			show_interface();	
			printf("haha process %s \n",song_path[song_index_g]);
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



