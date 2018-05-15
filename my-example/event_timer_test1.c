// Last Update:2014-05-24 23:57:47

#include <stdio.h>

#include <event.h>

struct event ev;
struct timeval tv;

struct event ev2;
struct timeval tv2;

struct event ev3;
struct timeval tv3;

void time_cb(int fd, short event, void *args)
{
	printf("first, timer wakeup\n");
	event_add(&ev, &tv);
}

void time_cb2(int fd, short event, void *args)
{
	printf("second, hi!\n");
	event_add(&ev2, &tv2);
}

void time_cb3(int fd, short event, void *args)
{
	printf("third, hello!\n");
	event_add(&ev3, &tv3);
}

void event_test()
{
	/* 初始化Reactor实例 */
	struct event_base *base = event_init();

	tv.tv_sec = 2;
	tv.tv_usec = 0;

	tv2.tv_sec = 4;
	tv2.tv_usec = 0;

	tv3.tv_sec = 10;
	tv3.tv_usec = 0;

	/* 初始化事件，设置回调和关注的事件 */
	evtimer_set(&ev, time_cb, NULL);
	evtimer_set(&ev2, time_cb2, NULL);
	evtimer_set(&ev3, time_cb3, NULL);

	/*
	 * 添加定时事件
	 * 对于定时事件，libevent使用小根堆管理,KEY为超时时间
	 * 对于signal和IO事件，libevent将其放入到等待链表中，
	 * 这是一个双向链表结构
	 */
	event_add(&ev, &tv);
	event_add(&ev2, &tv2);
	event_add(&ev3, &tv3);

	/* 对激活链表中的事件，调用事件的回调函数进行处理 */
	event_base_dispatch(base);
}

int main(int argc, char *argv[])
{
	event_test();
	return 0;
}
