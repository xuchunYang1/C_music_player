**Overview**

C语言音乐播放器,纯字符交互界面。

**准备环境**

安装开源软件[mplayer](https://mpv.io)

```shell
sudo apt-get install mplayer
```

**下载歌曲**（*放入下面的路径*）

```c
/home/yang/music/songs/木小雅 - 可能否.mp3
/home/yang/music/songs/宋冬野 - 斑马，斑马.mp3
/home/yang/music/songs/宋冬野 - 董小姐.mp3
/home/yang/music/songs/杨坤,郭采洁 - 答案.mp3
/home/yang/music/songs/赵雷 - 让我偷偷看你.mp3
/home/yang/music/songs/金玟岐 - 岁月神偷.mp3
```

**编译**

```shell
cd mplayerPro
cmake .
make 
```

---

**运行效果**

![](http://p4y9iazc7.bkt.clouddn.com/20180724094928.png)

------

**目前实现的功能**	

1. 自动播放、退出
2. 切换上一首、下一首
3. 快进、快退、暂停

**没有实现的功能**

1. 单曲循环、随机播放

------

**总结**

1.该项目在歌曲文件方面，设计了一个简单的`sqlite3`数据库存放它们的绝对路径，歌曲文件都是固定的，并且目前的歌曲文件也没有添加歌曲的功能，具有极大的极限性，望后期可以改进。

2.该项目用到多进程、有名管道、线程、信号量、互斥锁等Linux系统编程方面的一些知识。其中线程间通信还不是很熟练，而且代码里多多少少还存在一些小bug，希望后期能优化的更好。