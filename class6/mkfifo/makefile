.PHONY:all
all:server client
server:server.c
	gcc -o $@ $^
client:client.c
	gcc -o $@ $^

test: test.c
	gcc -o $@ $^
test1:test1.c
	gcc -o $@ $^

.PHONY : clean
clean:
	rm -f server client
