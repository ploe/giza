dir = ./giza/
drivers = $(dir)drivers
compile = cc

all: giza_exec

giza_exec:
	cc -g utils.c giza-exec.c -o giza-exec

install:
	cp -r $(dir) /etc/giza

clean:
	rm -rvf $(dir)

