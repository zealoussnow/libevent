// Last Update:2015-01-30 15:33:03

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <event.h>

#define unused __attribute__ ((unused))

int client_sd;


void client_callback(evutil_socket_t fd, short events, void *arg)
{
	printf("callback is called...\n");
	char buf[BUFSIZ] = {0};
	ssize_t retval = -1;
	struct event *ev = arg;

	event_add(ev, NULL);
	retval = read(fd, buf, BUFSIZ);
	printf("%ld\n", retval);
	if (retval > 0)
	{
		printf("%s\n", buf);
	}
	else if (retval < 0)
	{
		event_del(ev);
		perror("\033[0;31mread data from socket error\033[0m");
		close(fd);
		return ;
	}
	else
	{
		event_del(ev);
		fprintf(stdout, "\033[0;35mClient connection closed...\033[0m\n");
		close(fd);
		return ;
	}
}

int main(int argc, char *argv[])
{
	struct event client_event;
	int lisen_sd = socket(AF_INET, SOCK_STREAM, 0);
	if (lisen_sd == -1)
	{
		perror("Create socket error");
		exit(EXIT_FAILURE);
	}
	int flag = 1;
	setsockopt(lisen_sd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
	struct sockaddr_in s_addr;
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port	  = htons(9001);
	s_addr.sin_addr.s_addr = inet_addr("192.168.171.188");
	if ( (bind(lisen_sd, (struct sockaddr *)&s_addr, sizeof(s_addr))) == -1)
	{
		perror("Bind error");
		exit(EXIT_FAILURE);
	}
	if ( (listen(lisen_sd, 5)) == -1)
	{
		perror("Listen error");
		exit(EXIT_FAILURE);
	}
	event_init();
	while (1)
	{
		/* int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
		 * */
		struct sockaddr_in client_addr;
		memset(&client_addr, 0, sizeof(client_addr));
		socklen_t addrlen = sizeof(client_addr);
		client_sd = accept(lisen_sd, (struct sockaddr *)&client_addr, &addrlen);
		if (client_sd == -1)
		{
			perror("Accept error");
			exit(EXIT_FAILURE);
		}
		printf("accept a new client, ip: %s, port: %d\n",
				inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		memset(&client_event, 0, sizeof(client_event));
		event_set(&client_event, client_sd, EV_READ, client_callback, &client_event);
		event_set(&client_event, STDIN_FILENO, EV_READ, client_callback, &client_event);
		event_add(&client_event, NULL);
		event_dispatch();
	}

	return 0;
}
