.PHONY : clean

myls : myls.o
	gcc myls.o -o myls

%.o : %.c
	gcc -c $^ -o $@

clean :
	rm -vf *.o myls
