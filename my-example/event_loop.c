// Last Update:2015-10-15 15:51:21

#include <stdio.h>
#include <stdlib.h>

#include <event.h>

void run_base_with_ticks(struct event_base *base)
{
	struct timeval ten_sec;

	ten_sec.tv_sec = 10;
	ten_sec.tv_usec = 0;

	while (1)
	{
		event_base_loopexit(base, &ten_sec);
		event_base_dispatch(base);
		printf("tick...\n");
	}
}

int main(int argc, const char *argv[])
{
	struct event_base *base;

	base = event_base_new();
	if (!base)
	{
		fprintf(stderr, "Init libevent error\n");
		exit(EXIT_FAILURE);
	}
	run_base_with_ticks(base);
	//event_base_dump_events(base, stdout);
	event_base_free(base);
	printf("done\n");
	
	return 0;
}
