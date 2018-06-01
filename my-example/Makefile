CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = 
LDLIBS = -levent -lrt -lpthread

TARGET = event_base_backend event_loop event_server2 \
	 event_timer_test1 ht-test sigio socket_based_bufferevent \
	test event_exer event_server1 event_test evutil_test memory_alloc \
	signal socket_pair trivial_http_client


all : $(TARGET)

.PHONY : all clean

clean : 
	-rm -rfv $(TARGET) a.out
