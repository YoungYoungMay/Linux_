.PHONY:all
all:producer_consumer producer_consumer1 test test1

producer_consumer:producer_consumer.c
	gcc -o $@ $^ -lpthread

producer_consumer1:producer_consumer1.c 
	gcc -o $@ $^ -lpthread

test:test.c
	gcc -o $@ $^ -lpthread

test1:test1.c
	gcc -o $@ $^ -lpthread
 
.PHONY:clean
clean:
	rm producer_consumer producer_consumer1
