drivers = ./drivers/
compile = cc -g -O2 -I./murp -I./murmur3 -I./hashish ./murmur3/murmur3.o ./murp/murp.o ./hashish/hashish.o

all: giza_exec giza_user

giza_exec:
	$(compile) utils.c giza-exec.c -o giza-exec

giza_user:
	$(compile) utils.c giza-user.c -o $(drivers)giza-user 

install:
	cp -r /etc/giza

clean:
	rm -rvf $(drivers)

