CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}
CC=gcc

all: tcp_client tcp_server udp_client udp_server $(CUR_OBJS)

tcp_client:tcp_client.o
	$(CC) -o $@ $^

tcp_server:tcp_server.o
	$(CC) -o $@ $^

udp_client:udp_client.o
	$(CC) -o $@ $^

udp_server:udp_server.o
	$(CC) -o $@ $^

$(CUR_OBJS):%.o:%.c
	$(CC) $(INCLUDE) -c $^ -o ./$@
