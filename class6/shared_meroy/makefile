ser=server
cli=client
cc=gcc

all:$(ser)	$(cli)

$(ser):server.c	
	$(cc)	-o	$@	$^
$(cli):client.c	
	$(cc)	-o	$@	$^

.PHONY:clean
clean:
	rm	client	server
