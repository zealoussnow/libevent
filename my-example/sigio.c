// Last Update:2014-03-30 02:01:55

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_LEN 100
void input_handler(int num)
{
	char data[MAX_LEN];
	int len;
	//读取并输出STDIN_FILENO上的输入
	len = read(STDIN_FILENO, &data, MAX_LEN);
	data[len] = 0;
	printf("input available:%s\n", data);
}


int main(int argc, char *argv[])
{
	int oflags;

	//启动信号驱动机制
	signal(SIGIO, input_handler);
	fcntl(STDIN_FILENO, F_SETOWN, getpid());
	oflags = fcntl(STDIN_FILENO, F_GETFL);
	fcntl(STDIN_FILENO, F_SETFL, oflags | FASYNC);

	//最后进入一个死循环，程序什么都不干了，只有信号能激发input_handler的运行
	while (1);

	return 0;
}

