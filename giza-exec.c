#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct callback {
	char *key;
	void (*value)();
} callback;

static void call_drivers() {
	puts("list global and local drivers");
	exit(0);	
}

static void call_help() {
	puts("giza - A shell/JSON driven *nix configuration management tool.\n");

	puts("usage:\n------");
	puts(" giza [driver] 'json'");	
	puts("    You can pass a JSON structure to the giza driver as an arg.\n");	
	puts("  giza [driver]");	
	puts("    If a JSON arg isn't defined the giza driver will expect one from STDIN.\n");

	puts("options:\n--------");
	puts("  --drivers\t\tLists the global and local drivers.");
	puts("  --help\t\tPrints the help information.");
	puts("  --version\t\tPrints the version of giza you are using.");
		
	exit(0);	
}

static void call_version() {
	puts("giza-exec (pre-alpha)");	
	exit(0);	
}

static callback options[] = {
	{"--drivers", call_drivers},
	{"--help", call_help},
	{"--version", call_version},
	{NULL, NULL},
};


static int parse_args(char *arg) {
	callback *c;
	for(c = options; c->key != NULL; c++) {
		if (strcmp(c->key, arg) == 0) c->value();
	}
}

char *myname;

int globerr(const char *path, int eerrno) {
	fprintf(stderr, "%s: %s\n", myname, path);
	return 0;
}


int main(int argc, char *argv[]) {
	myname = argv[0];

	int i;
	for (i = 0; i < argc; i++) {
		parse_args(argv[i]);	
	}

	glob_t results;
	int err = glob("/home/atkinsonm/giza/*", 0, globerr, &results);
	switch (err) {
		case GLOB_ABORTED: puts("glob aborted lol!"); break;
		case GLOB_NOMATCH: puts("glob nomatch lol!"); break;
		case GLOB_NOSPACE: puts("glob nospace lol!"); break;
	}

	for(i = 0; i < results.gl_pathc; i++) puts(results.gl_pathv[i]);

	globfree(&results);
	puts("done!");

	execlp("sort", "sort", NULL, NULL);

	return 0;
}
