.PHONY:all
all:tcp_server tcp_client tcp_server1 tcp_server2

tcp_server:tcp_server.c
	gcc -o $@ $^

tcp_client:tcp_client.c
	gcc -o $@ $^

tcp_server1:tcp_server1.c
	gcc -o $@ $^

tcp_server2:tcp_server2.c
	gcc -o $@ $^ -lpthread

.PHONY:clean
clean:
	rm -f tcp_server tcp_client
