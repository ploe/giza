dir = ./giza/
drivers = $(dir)drivers

all: giza_exec


giza_exec:
	cc utils.c giza-exec.c -o giza-exec

install:
	cp -r $(dir) /etc/giza

clean:
	rm -rvf $(dir)

