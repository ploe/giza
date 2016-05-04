drivers = drivers/
compile = cc -g -O2 -L./murp/ -I./murp/ -L./murmur3/ -I./murmur3/

all: dirs giza_exec giza_user

giza_exec:
	$(compile) utils.c giza-exec.c -o giza-exec

giza_user:
	$(compile) utils.c giza-user.c -o $(drivers)giza-user 

install:
	cp -r /etc/giza

clean:
	rm -rvf $(drivers)

