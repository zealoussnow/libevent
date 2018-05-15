// Last Update:2014-05-24 22:19:19

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <event.h>

static void read_sth(evutil_socket_t fd, short event, void *arg);
void *start_routine(void *args);

int main(int argc, char *argv[])
{
	int listener, acceptor, connector;
	struct sockaddr_in listen_addr;
	struct sockaddr_in connect_addr;

	memset(&listen_addr, 0, sizeof(struct sockaddr_in));
	memset(&connect_addr, 0, sizeof(struct sockaddr_in));

	/* 创建监听socket */
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener == -1)
	{
		perror("create listener failed");
		exit(EXIT_FAILURE);
	}

	/* 绑定到本地环回地址 */
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	listen_addr.sin_port = 0;
	if ( bind(listener, (struct sockaddr *)&listen_addr,
				sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind error");
		exit(EXIT_FAILURE);
	}

	if ( listen(listener, 1) == -1)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	/* 创建连接socket */
	connector = socket(AF_INET, SOCK_STREAM, 0);
	if (connector == -1)
	{
		perror("create connector failed");
		exit(EXIT_FAILURE);
	}
	socklen_t size = sizeof(connect_addr);
	if ( getsockname(listener, (struct sockaddr *)&connect_addr, &size)
			== -1)
	{
		perror("getsockname error");
		exit(EXIT_FAILURE);
	}
	char ip_buffer[128];
	memset(ip_buffer, 0, sizeof(ip_buffer));
	printf("ip: %s\n", inet_ntop(AF_INET, &connect_addr.sin_addr, ip_buffer, sizeof(ip_buffer)));
	printf("port: %d\n", htons(connect_addr.sin_port));

	/* 连接到监听socket监听的端口 */
	if ( connect(connector, (struct sockaddr *)&connect_addr,
				sizeof(struct sockaddr_in)) == -1)
	{
		perror("connect error");
		exit(EXIT_FAILURE);
	}
	size = sizeof(listen_addr);
	/* 取得连接成功后返回的socket */
	acceptor = accept(listener, (struct sockaddr *)&listen_addr, &size);
	if (acceptor < 0)
	{
		perror("accept error");
		exit(EXIT_FAILURE);
	}
	close(listener);

	if ( getsockname(connector, (struct sockaddr *)&connect_addr, &size)
			== -1)
	{
		perror("getsockname error");
		exit(EXIT_FAILURE);
	}
	memset(ip_buffer, 0, sizeof(ip_buffer));
	printf("ip: %s\n", inet_ntop(AF_INET, &connect_addr.sin_addr, ip_buffer, sizeof(ip_buffer)));
	printf("port: %d\n", htons(connect_addr.sin_port));
	if (size != sizeof (connect_addr)
			|| listen_addr.sin_family != connect_addr.sin_family
			|| listen_addr.sin_addr.s_addr != connect_addr.sin_addr.s_addr
			|| listen_addr.sin_port != connect_addr.sin_port)
	{
		fprintf(stderr, "%s\n", "Maybe something error");
	}

#if 1 //事件驱动
	struct event presist_read;
	event_init();
	event_set(&presist_read, acceptor, EV_READ | EV_PERSIST, read_sth, &presist_read); //注册acceptor的永久读事件
	event_add(&presist_read, NULL);

	pthread_t tid;
	pthread_create(&tid, NULL, start_routine, (void *)&connector);
	if ( (pthread_join(tid, NULL)) == 0)
	{
		printf("the new thread exit\n");
	}
	event_dispatch();
#endif

#if 0 //信号驱动IO，是不是想错了？哦，要设置FASYNC标志啊，不然默认的SIGIO没了，为什么？
	// 还是SIGIO在TCP中没有作用？
	int flags = fcntl(connector, F_GETFD);
	flags |= O_NONBLOCK | O_ANSY;
	fcntl(connector, F_SETFD, flags);
	struct event signal_io;
	struct event_base *base = event_base_new();
	event_assign(&signal_io, base, SIGIO, EV_SIGNAL | EV_PERSIST, read_sth, &signal_io);
	event_add(&signal_io, NULL);
	pthread_t tid;
	pthread_create(&tid, NULL, start_routine, (void *)&connector);
	if ( (pthread_join(tid, NULL)) == 0)
	{
		printf("the new thread exit\n");
	}
	event_base_dispatch(base);
	event_base_free(base);
#endif

	return 0;
}

void *start_routine(void *args)
{
	int connector = (int)*(int *)args;
	char write_buf[] = "Hello,World!";
	printf("thread routine write something\n");
	write(connector, write_buf, sizeof(write_buf));

	return NULL;
}

static void read_sth(evutil_socket_t fd, short event, void *arg)
{
	int len;
	char buf[255];
	len = read(fd, buf, sizeof(buf) - 1);

	if (len == -1) {
		perror("read");
		return;
	} else if (len == 0) {
		fprintf(stderr, "Connection closed\n");
		return;
	}

	buf[len] = '\0';
	printf("read_sth: %s\n", buf);
}
