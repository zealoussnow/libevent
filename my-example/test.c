#include <assert.h>
#include <stdio.h>
#include <event.h>

#define poll_check_ok(pop)

struct event_base *ev_base;

int main(int argc, const char *argv[])
{
	/*int events = EV_READ;*/
	/*poll_check_ok(events);*/
	/*printf("%d\n", EV_SIGNAL);*/

	/*assert((events & EV_SIGNAL) == 0);*/
	
	ev_base = event_init();

	return 0;
}
