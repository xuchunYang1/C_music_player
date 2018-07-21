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
pthread_t tid;  //线程ID
sem_t sem_exit_all_thread; //线程退出信号
pthread_mutex_t mutex_for_index = PTHREAD_MUTEX_INITIALIZER;
//mplayer 命令 以'\n'作为结束
char pause_play[] = "pause\n";
char valume_off[] = "mute 1\n";
char valume_on[] = "mute 0\n";
//char volume_add[] = "volume 1\n";
//char volume_sub[] = "volume 1\n";   
char fast_forward[] = "seek 10\n";
char fast_backward[] = "seek -10\n";
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
#if 0
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
#endif
	
	unlink("./cmdfifo");
	res = mkfifo("./cmdfifo", 0777);
	if (res != 0)
	{
		fprintf(stderr, "Could not create fifo %s\n", "./cmdfifo");
		exit(1);
	}
	else
	{
			printf("fifo success res = %d\n", res);	
	}
	
	pipe_fd = open("./cmdfifo", O_RDWR|O_NONBLOCK); //可读可写非阻塞打开管道
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
	pid_t ret;
	if (pid_play_g > 0)//pid must greater than 0 : fix system 
	{
		printf("kill pid %d \n",pid_play_g);
		kill(pid_play_g, 9);
		pid_play_g = 0;
	}

	pid_play_g = fork();
	if (pid_play_g < 0)
	{
		printf("fork error\n");
		exit(1);
	}
	else if (pid_play_g == 0) //子进程  "-idle"播放完之后不退出
	{
		execl("/usr/bin/mplayer", "mplayer", "-slave", "-quiet", 
					"-input", "file=./cmdfifo", song_path[song_index_g], NULL);
		pid_play_g = 0;
		printf("son exit\n\n\n");	
	}
}
/*******************************************************************************/
/*名称： do_auto_play
/*描述：  播放歌曲
/*作成日期：2018/07/17
/*参数：
/*返回值：
/*作者：yang
/*******************************************************************************/
void init_sem()
{
	int ret;
	ret = sem_init(&sem_exit_all_thread, 0, 0);
	if (ret != 0)
	{
		printf("sem_init error\n");
	}
}

void init_mutex()
{
	pthread_mutex_init(&mutex_for_index, NULL);
}
void *thread_process(void *arg) //线程函数
{
	int ret;
	int status;
	struct timespec ts; //定义一个时间结构体变量
	printf("in thread\n");
	while (1)
	{	
		printf("index = %d\n", song_index_g);
	
		if (pid_play_g > 0) //有歌曲正在播放，阻塞等待
		{
			printf("waitpid pid_play_g = %d\n", pid_play_g);
			waitpid(pid_play_g, &status, 0);
		}
		
		clock_gettime(CLOCK_REALTIME, &ts); //
		ts.tv_sec += 1;
		//等待退出信号　非阻塞
		ret = sem_timedwait(&sem_exit_all_thread, &ts);

		if (ret == -1)
		{
		
		}
		else
		{
			printf("receive exit signal\n");
			break;
		}
		
		do_next();

	}
	pthread_exit(0);
}
void do_auto_play()
{	
	pthread_create(&tid, NULL, thread_process, NULL);
	
	//pthread_join(tid, NULL); //等待线程退出
	//sem_destroy(&sem_exit_all_thread);
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
	printf("PREVIEW!\n");
	pthread_mutex_lock(&mutex_for_index);
	--song_index_g;
	
	if (song_index_g < 0)
	{
		song_index_g = MAXSONGNUM - 1;
	}
	
	do_play();
	pthread_mutex_unlock(&mutex_for_index);
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
	printf("NEXT!\n");
	pthread_mutex_lock(&mutex_for_index);
	++song_index_g;
	if (song_index_g >= MAXSONGNUM)
	{
		song_index_g = 0;
	}

	do_play();
	pthread_mutex_unlock(&mutex_for_index);
}
/*****************************************************************************
/*名称： do_forward
/*描述：  快进10秒
/*作成日期：2018/07/17
/*参数：
/*返回值：
/*作者：yang
/*******************************************************************************/
static int fast = 0; //test
void do_forward()
{	
	printf("FAST_FORWARD\n");
	++fast;
	int ret;
	ret = write(pipe_fd, fast_forward, strlen(fast_forward)); //sizeof(fast_forward)
	if (ret == -1)						//此处是strlen,不计算最后一个'\0',而sizeof计算的是整个存储的长度
	{
		printf("write failed\n");
		exit(1);
	}
	//printf("strlen ret = %d\n", ret); 
	//printf("sizeof ret = %d\n", sizeof(fast_forward));
	//printf("fast number = %d\n", fast);
}
/*****************************************************************************
/*名称： do_rewind
/*描述：  快退10秒
/*作成日期：2018/07/17
/*参数：
/*返回值：
/*作者：yang
/*******************************************************************************/
void do_rewind()
{	
	int ret;
	ret = write(pipe_fd, fast_backward, strlen(fast_backward));
	if (ret == -1)
	{
		printf("write failed\n");
		exit(1);
	}
	//printf("ret = %d\n", ret);
}
/*****************************************************************************
/*名称： do_pause
/*描述：  pause
/*作成日期：2018/07/17
/*参数：
/*返回值：
/*作者：yang
/*******************************************************************************/
//static int pause_num = 0; //test
void do_pause()
{
	//++pause_num;
	int ret;
	ret = write(pipe_fd, pause_play, strlen(pause_play));
	if (ret == -1)
	{
		printf("write failed\n");
		exit(1);
	}
	//printf("ret = %d\n", ret);
	//printf("pause number = %d\n", pause_num);
}
void do_exit()
{
	sem_post(&sem_exit_all_thread);
}
/*****************************************************************************
/*名称： menu
/*描述： 播放菜单
/*作成日期：2015/4/30
/*参数：无
/*返回值：
/*作者：yang
/***********
/********************************************************************/
void menu()
{
	char ensure_exit = 0;
	char exit_flag = 1;
	char option = 'x';  //初始化为x,因为菜单里有0
	bool auto_play_flag = false;
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
					 		close(pipe_fd); //关闭管道
					 		do_exit();
					 		//pthread_exit(NULL);
					 		kill(pid_play_g, 9);
					 		pid_play_g = 0;
					 		pthread_join(tid, NULL);
					 		// 杀死相关子进程

					 		printf("已退出, 欢迎使用!\n");
					 	}
			break;
			case PLAY: // 开始播放
				//printf("PLAY!\n");
				if (auto_play_flag)
				{
					do_exit();
					pthread_join(tid, NULL);
				}
				auto_play_flag = true;
				do_auto_play();
			break;
			case SWITCH_PREVIEW:  // 上一曲
				//printf("PREVIEW!\n");
				do_preview();
				//删除自动播放进程
				do_exit();
		 		pthread_join(tid, NULL);
		 		do_auto_play();
			break;
			case PAUSE:  // 暂停
				printf("PAUSE\n");
				do_pause();
			break;
			case SWITCH_NEXT:  //下一曲
				if(auto_play_flag)
				{
					//printf("NEXT!\n");
					do_next();
					//删除自动播放进程
					do_exit();
			 		pthread_join(tid, NULL);
			 		do_auto_play();
				}
				else
				{
					//printf("NEXT!\n");
					do_next();
			 		do_auto_play();
			 		auto_play_flag = true;
				}
			break;
			case FAST_FORWARD:  //快进
				//printf("FAST_FORWARD\n");
				do_forward();
				
			break;
			case FAST_BACKWARD:  //快退
				//printf("FAST_BACKWARD\n");
				do_rewind();
				
			break;
			case SINGLE_LOOP: //单曲播放

			break;
			case LIST_LOOP: //循环播放

			break;
			case RANDOM:  //随机播放

			break;
			//case HELPME: // 帮助 枚举不能与宏定义同名
			//show_interface();	
			//printf("haha process %s \n",song_path[song_index_g]);
			//break;
			
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



