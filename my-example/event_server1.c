/*
   This exmple program provides a trivial server program that listens for TCP
   connections on port 9995.  When they arrive, it writes a short message to
   each client connection, and closes each connection once it is flushed.

   Where possible, it exits cleanly in response to a SIGINT (ctrl-c).
*/


#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>

#ifdef _XOPEN_SOURCE_EXTENDED
#include <arpa/inet.h>
#endif

#include <sys/socket.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <arpa/inet.h>

static const int PORT = 9000;

static void listener_cb(struct evconnlistener *, evutil_socket_t,
		struct sockaddr *, int socklen, void *);
static void conn_readcb(struct bufferevent *, void *);
static void conn_writecb(struct bufferevent *, void *);
static void conn_eventcb(struct bufferevent *, short, void *);
static void signal_cb(evutil_socket_t, short, void *);

int main(int argc, char **argv)
{
	struct event_base *base;
	struct evconnlistener *listener;
	struct event *signal_event;

	struct sockaddr_in sin;

	base = event_base_new();
	if (!base)
	{
		fprintf(stderr, "Could not initialize libevent!\n");
		return 1;
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	/*
	 * Create a new socket, bind it, and set the socket options
	 */
	listener = evconnlistener_new_bind(base, listener_cb, (void *)base,
			LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
			(struct sockaddr*)&sin,
			sizeof(sin));

	if (!listener)
	{
		fprintf(stderr, "Could not create a listener!\n");
		return 1;
	}

	signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);

	if (!signal_event || event_add(signal_event, NULL)<0)
	{
		fprintf(stderr, "Could not create/add a signal event!\n");
		return 1;
	}

	event_base_dispatch(base);

	evconnlistener_free(listener);
	event_free(signal_event);
	event_base_free(base);

	printf("done\n");
	return 0;
}

/* 每次客户端连接上时调用 */
static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
		struct sockaddr *sa, int socklen, void *user_data)
{
	/* 这里是如何传递客户端的信息的，怎么得到的客户端ip和端口？ */

	struct sockaddr_in *sin = (struct sockaddr_in *)sa;
	printf("a new Connection, ip: %s, port: %d\n", inet_ntoa(sin->sin_addr), ntohs(sin->sin_port));
	struct event_base *base = user_data;
	struct bufferevent *bev;

	/* XXX Close on free, what's the meaning? */
	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	if (!bev)
	{
		fprintf(stderr, "Error constructing bufferevent!");
		event_base_loopbreak(base);
		return;
	}

	bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
	bufferevent_enable(bev, EV_READ | EV_WRITE);
	/*bufferevent_enable(bev, EV_WRITE);*/
	printf("hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n");
}

static void conn_writecb(struct bufferevent *bev, void *user_data)
{
	char buffer[BUFSIZ] = {0};
	printf(">> ");
	fgets(buffer, BUFSIZ - 1, stdin);
	bufferevent_write(bev, buffer, BUFSIZ - 1);
	struct evbuffer *output = bufferevent_get_output(bev);
	if (evbuffer_get_length(output) == 0)
	{
		printf("flushed answer\n");
		bufferevent_free(bev);
	}
}

static void conn_readcb(struct bufferevent *bev, void *user_data)
{
	char buffer[BUFSIZ] = {0};
	size_t sz = bufferevent_read(bev, buffer, BUFSIZ - 1);
	if (sz > 0)
	{
		printf("buffer: %s\n", buffer);
	}
}

static void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
	if (events & BEV_EVENT_EOF)
	{
		printf("Connection closed.\n");
	}
	else if (events & BEV_EVENT_ERROR)
	{
		printf("Got an error on the connection: %s\n",
				strerror(errno));/*XXX win32*/
	}
	/* None of the other events can happen here, since we haven't enabled
	 * timeouts */
	bufferevent_free(bev);
}

static void signal_cb(evutil_socket_t sig, short events, void *user_data)
{
	struct event_base *base = user_data;
	struct timeval delay = { 2, 0 };

	printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

	event_base_loopexit(base, &delay);
}
