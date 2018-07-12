.PHONY:all
all:server client

server:server.c
	gcc -o $@ $^ -lpthread

client:client.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -f server client
