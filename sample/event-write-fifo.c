/*
 * This sample code shows how to use Libevent to read from a named pipe.
 * XXX This code could make better use of the Libevent interfaces.
 *
 * XXX This does not work on Windows; ignore everything inside the _WIN32 block.
 *
 * On UNIX, compile with:
 * cc -I/usr/local/include -o event-read-fifo event-read-fifo.c \
 *     -L/usr/local/lib -levent
 */

#include <event2/event-config.h>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
#include <sys/queue.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#else
#include <winsock2.h>
#include <windows.h>
#endif
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <event2/event.h>

static void
fifo_write(evutil_socket_t fd, short event, void *arg)
{
	char buf[255];
	int len;
	struct event *ev = arg;
#ifdef _WIN32
	DWORD dwBytesRead;
#endif

	fprintf(stderr, "fifo_read called with fd: %d, event: %d, arg: %p\n",
	    (int)fd, event, arg);
#ifdef _WIN32
	len = ReadFile((HANDLE)fd, buf, sizeof(buf) - 1, &dwBytesRead, NULL);

	/* Check for end of file. */
	if (len && dwBytesRead == 0) {
		fprintf(stderr, "End Of File");
		event_del(ev);
		return;
	}

	buf[dwBytesRead] = '\0';
#else
	memset(buf, 'x', sizeof(buf) - 1);
	buf[sizeof(buf) - 1] = '\0';

	len = write(fd, buf, sizeof(buf) - 1);

	if (len <= 0) {
		if (len == -1)
			perror("read");
		else if (len == 0)
			fprintf(stderr, "Connection closed\n");
		event_del(ev);
		event_base_loopbreak(event_get_base(ev));
		return;
	}
#endif
	sleep(1);
}

/* On Unix, cleanup event.fifo if SIGINT is received. */
#ifndef _WIN32
static void
signal_cb(evutil_socket_t fd, short event, void *arg)
{
	struct event_base *base = arg;
	event_base_loopbreak(base);
}
#endif

int
main(int argc, char **argv)
{
	struct event *evfifo;
	struct event_base* base;
#ifdef _WIN32
	HANDLE socket;
	/* Open a file. */
	socket = CreateFileA("test.txt",	/* open File */
			GENERIC_WRITE,		/* open for reading */
			0,			/* do not share */
			NULL,			/* no security */
			OPEN_EXISTING,		/* existing file only */
			FILE_ATTRIBUTE_NORMAL,	/* normal file */
			NULL);			/* no attr. template */

	if (socket == INVALID_HANDLE_VALUE)
		return 1;

#else
	struct event *signal_int;
	struct stat st;
	const char *fifo = "event.fifo";
	int socket;

	if (lstat(fifo, &st) == 0) {
		if ((st.st_mode & S_IFMT) == S_IFREG) {
			errno = EEXIST;
			perror("lstat");
			exit(1);
		}
	}

	socket = open(fifo, O_WRONLY | O_NONBLOCK, 0);

	if (socket == -1) {
		perror("open");
		exit(1);
	}

	fprintf(stderr, "Write data to %s\n", fifo);
#endif
	/* Initalize the event library */
	base = event_base_new();

	/* Initalize one event */
#ifdef _WIN32
	evfifo = event_new(base, (evutil_socket_t)socket, EV_READ|EV_PERSIST, fifo_read,
                           event_self_cbarg());
#else
	/* catch SIGINT so that event.fifo can be cleaned up */
	signal_int = evsignal_new(base, SIGINT, signal_cb, base);
	event_add(signal_int, NULL);

	evfifo = event_new(base, socket, EV_WRITE|EV_PERSIST, fifo_write,
                           event_self_cbarg());
#endif

	/* Add it to the active events, without a timeout */
	event_add(evfifo, NULL);

	event_base_dispatch(base);
	printf("event dispatch exit, will free now...\n");
	event_base_free(base);
#ifdef _WIN32
	CloseHandle(socket);
#else
	close(socket);
#endif
	libevent_global_shutdown();
	return (0);
}

