.PHONY:all
all:myhttpd test

myhttpd:myhttpd.c
	gcc -o $@ $^

test:test.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -f myhttpd test
