// Last Update:2014-03-30 01:14:20

#include <stdio.h>
#include <stdlib.h>

#include <event.h>

struct event ev;
struct timeval tv;

void time_cb(int fd, short event, void *args)
{
	printf("timer wakeup\n");
	event_add(&ev, &tv);
}

void event_test()
{
	/* 初始化Reactor实例 */
	struct event_base *base = event_init();
	tv.tv_sec = 0;
	tv.tv_usec = 100000;

	/* 初始化事件，设置回调和关注的事件 */
	evtimer_set(&ev, time_cb, NULL);

	/*
	 * 添加定时事件
	 * 对于定时事件，libevent使用小根堆管理,KEY为超时时间
	 * 对于signal和IO事件，libevent将其放入到等待链表中，
	 * 这是一个双向链表结构
	 */
	event_add(&ev, &tv);
	printf("%d\n", event_get_fd(&ev));
	printf("%lu\n", event_get_struct_event_size());

	/* 当然，当事件被删除后，它就不能投递到事件的激活链表中 */
	//event_del(&ev);

	FILE *output = fopen("event_base.dump", "w");
	if (output == NULL)
	{
		perror("can not open the specified file");
		exit(EXIT_FAILURE);
	}
	event_base_dump_events(base, output);
	fclose(output);
	/* 对激活链表中的事件，调用事件的回调函数进行处理 */
	event_base_dispatch(base);
}

int main(int argc, char *argv[])
{
	event_test();
	return 0;
}
