#include "giza.h"
#include "murmur3.h"

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


static void parse_args(char *arg) {
	callback *c;
	for(c = options; c->key != NULL; c++) {
		if (strcmp(c->key, arg) == 0) c->value();
	}
}

static int spit(char *path) {
//	size_t len = strlen(path) - 1;
//	if (path[len] != '/') {
//		puts(path);				
//	}

	return giz_CONTINUE;
}

#define UINT128_LENGTH 16

typedef struct KVPair {
	uint8_t hash[UINT128_LENGTH];
	char *key;
	void *value;
	int (*destruct)(void *);
	struct KVPair *prev, *next;	
} KVPair;

typedef struct Map {
	KVPair *buckets[UINT8_MAX];
} Map;

static void SetHash(char *key, void *out) {
	MurmurHash3_x64_128(key, strlen(key), getpid(), out);
}

static KVPair *FindPair(Map *map, char *key) {
	uint8_t hash[UINT128_LENGTH];
	SetHash(key, hash);

	KVPair *top = map->buckets[hash[0]], *pair;
	for (pair = top; pair != NULL; pair = pair->next) {
		if (memcmp(hash, pair->hash, sizeof(uint8_t) * UINT128_LENGTH) == 0)
			if (strcmp(key, pair->key) == 0) return pair;
	}
	return NULL;
}

KVPair *NewKVPair(Map *map, char *key) {
	KVPair *pair = calloc(1, sizeof(KVPair));
	if (pair) {
		SetHash(key, pair->hash);

		pair->key = calloc(strlen(key) + 1, sizeof(char));
		if (!pair->key) return NULL;
		strcpy(pair->key, key);


		KVPair *top = map->buckets[pair->hash[0]];
		if (top) top->prev = pair;
		pair->next = top;

		map->buckets[pair->hash[0]] = pair;
	}

	return pair;
}

int Set(Map *map, char *key, void *value, int (*destruct)(void *)) {
	KVPair *pair;
	if ((pair = FindPair(map, key)) == NULL) pair = NewKVPair(map, key);

	if (pair) {
		pair->value = value;
		pair->destruct = destruct;
		return 1;

	}
	return 0;
}

#undef UINT128_LENGTH


int main(int argc, char *argv[]) {
	Map *map = calloc(1, sizeof(Map));
	Set(map, "hello, world", NULL, NULL);
	Set(map, "myke my boy", NULL, NULL);
	Set(map, "hello, world", NULL, NULL);
	Set(map, "myke my boy", NULL, NULL);
	free(map);

	int i;
	for (i = 0; i < argc; i++) {
		parse_args(argv[i]);	
	}

	int err = giz_EachGlob("~/*", spit);
	switch (err) {
		case GLOB_ABORTED: puts("glob aborted lol!"); break;
		case GLOB_NOMATCH: puts("glob nomatch lol!"); break;
		case GLOB_NOSPACE: puts("glob nospace lol!"); break;
	}


	execlp("./drivers/giza-user", "giza-user", NULL, NULL);

	return 0;
}
