#include "giza.h"

typedef struct callback {
	char *key, *desc;
	void (*value)();
} callback;

static callback options[];

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
	callback *c;
	for (c = options; c->key != NULL; c++) printf("  %s\t\t%s\n", c->key, c->desc);
	
	exit(0);	
}

static void call_version() {
	printf("giza-exec (%s %.2f)\n", _GIZA_VERSION_NAME, _GIZA_VERSION_NUMBER);	
	exit(0);	
}

static callback options[] = {
	{"--drivers", "Lists the global and local drivers.", call_drivers},
	{"--help", "Prints the help information.", call_help},
	{"--version", "Prints the version of giza you are using.", call_version},
	{NULL, NULL, NULL},
};


static int parse_args(char *arg) {
	callback *c;
	for(c = options; c->key != NULL; c++) {
		if (strcmp(c->key, arg) == 0) c->value();
	}
}

static int spit(char *path) {
	size_t len = strlen(path) - 1;
	if (path[len] != '/') {
		puts(path);				
	}

	return giz_CONTINUE;
}

#define BUFFER_SIZE 1024
char *giz_AbsorbFile(int fd) {
	int n, len = 0, size = BUFFER_SIZE;
	char buffer[BUFFER_SIZE];
	char *str = calloc(BUFFER_SIZE, sizeof(char)*size);

	while ((n = read(fd, buffer, BUFFER_SIZE)) > 0) {
		// Double the size of str if we've filled it up with char.
		if ((len + n) > size) {
			size *= 2;
			char *tmp = realloc(str, sizeof(char) * size);
			if (!tmp) {
				free(str);
				return NULL;
			}

		}	
		memcpy(str + len, buffer, sizeof(char)*n);
		len += n;
	}
	return str;
}
#undef BUFFER_SIZE

int main(int argc, char *argv[]) {

//	int i;
//	for (i = 0; i < argc; i++) {
//		parse_args(argv[i]);	
//	}


	char *str= giz_AbsorbFile(STDIN_FILENO);
	puts(str);
	free(str);

	exit(0);

	int err = giz_EachGlob("~/*", spit);
	switch (err) {
		case GLOB_ABORTED: puts("glob aborted lol!"); break;
		case GLOB_NOMATCH: puts("glob nomatch lol!"); break;
		case GLOB_NOSPACE: puts("glob nospace lol!"); break;
	}


	execlp("sort", "sort", NULL, NULL);

	return 0;
}
