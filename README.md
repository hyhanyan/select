# select


##
select 程序主要的作用：

1. 程序使用了一个数组fd_A，通信开始后把需要通信的多个socket描述符都放入此数组。

2. 首先生成一个叫sock_fd的socket描述符，用于监听端口。

3. 将sock_fd和数组fd_A中不为0的描述符放入select将检查的集合fdsr。

4. 处理fdsr中可以接收数据的连接。如果是sock_fd，表明有新连接加入，将新加入连接的socket描述符放置到fd_A。

int select(int nfds, fd_set *restrict readfds,fd_set *restrict writefds, fd_set *restrict errorfds,struct timeval *restrict timeout);

void FD_CLR(int fd, fd_set *fdset);		//清除一个fd_set变量的指定位
void FD_SET(int fd, fd_set *fdset);		//设置一个fd_set变量的指定位
void FD_ZERO(fd_set *fdset);			//将一个指定的fd_set变量所有位设置为0
int FD_ISSET(int fd, fd_set *fdset);	//测试一个指定位是否设置


struct timeval {
	long	tv_sec;		/* seconds */
	long	tv_usec;	/* microseconds */
};


select 一般的步骤，
每次循环，要先执行FD_ZERO,然后在设置，执行FD_SET，在执行select函数，再接着执行FD_ISSET,看看有哪些事件出现，当有监听的时间关闭时，用FD_CLR来清除。
