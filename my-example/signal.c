// Last Update:2015-10-19 13:57:03

#include <stdio.h>
#include <stdlib.h>
#include <evutil.h>

int main(int argc, const char *argv[])
{
	evutil_socket_t sd[2];
	int ret = evutil_socketpair(AF_INET, SOCK_STREAM, 0, sd);
	if (ret == -1)
	{
		//event_sock_err(1, -1, "%s: socketpair", __func__);
		fprintf(stderr, "Create socket pair error!\n");
		return -1;
	}
	else
	{
		evutil_make_socket_closeonexec(sd[0]);
		evutil_make_socket_closeonexec(sd[1]);
	}
	evutil_make_socket_nonblocking(sd[0]);
	
	return 0;
}
