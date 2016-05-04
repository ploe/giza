drivers = drivers/
compile = cc -g -O2

all: dirs giza_exec giza_user

dirs:
	mkdir -p $(drivers)

giza_exec:
	$(compile) utils.c giza-exec.c -o giza-exec

giza_user:
	$(compile) utils.c giza-user.c -o $(drivers)giza-user

install:
	cp -r /etc/giza

clean:
	rm -rvf $(drivers)

