.PHONY:all
all:thread_create thread_wait thread_exit thread_detach thread_id

thread_create:thread_create.c
	gcc -o $@ $^ -lpthread
thread_wait:thread_wait.c
	gcc -o $@ $^ -lpthread
thread_exit:thread_exit.c
	gcc -o $@ $^ -lpthread
thread_detach:thread_detach.c
	gcc -o $@ $^ -lpthread
thread_id:thread_id.c
	gcc -o $@ $^ -lpthread

.PHONY:clean
clean:
	rm -f mythread
