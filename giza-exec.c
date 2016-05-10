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

typedef struct ish_KVPair {
	uint8_t hash[UINT128_LENGTH];
	char *key;
	void *value;
	int (*destruct)(void *);
	struct ish_KVPair *prev, *next;	
} ish_KVPair;

typedef struct ish_Map {
	ish_KVPair *buckets[UINT8_MAX];
} ish_Map;

static void SetHash(char *key, void *out) {
	MurmurHash3_x64_128(key, strlen(key), getpid(), out);
}

static ish_KVPair *FindPair(ish_Map *map, char *key) {
	uint8_t hash[UINT128_LENGTH];
	SetHash(key, hash);

	ish_KVPair *top = map->buckets[hash[0]], *pair;
	for (pair = top; pair != NULL; pair = pair->next) {
		if (memcmp(hash, pair->hash, sizeof(uint8_t) * UINT128_LENGTH) == 0)
			if (strcmp(key, pair->key) == 0) return pair;
	}
	return NULL;
}

ish_KVPair *ish_NewKVPair(ish_Map *map, char *key) {
	ish_KVPair *pair = calloc(1, sizeof(ish_KVPair));
	if (pair) {
		SetHash(key, pair->hash);

		pair->key = calloc(strlen(key) + 1, sizeof(char));
		if (!pair->key) {
			free(pair);
			return NULL;
		}
		strcpy(pair->key, key);


		ish_KVPair *top = map->buckets[pair->hash[0]];
		if (top) top->prev = pair;
		pair->next = top;

		map->buckets[pair->hash[0]] = pair;
	}

	return pair;
}

int ish_MapSetWithDestruct(ish_Map *map, char *key, void *value, int (*destruct)(void *)) {
	ish_KVPair *pair;
	if ((pair = FindPair(map, key)) == NULL) pair = ish_NewKVPair(map, key);

	if (pair) {
		pair->value = value;
		pair->destruct = destruct;
		return 1;

	}
	return 0;
}

#define ish_MapSet(map, key, value) ish_MapSetWithDestruct(map, key, value, NULL)

void ish_MapProbePairs(ish_Map *map, int (*func)(char *, void *, void *), void *probe) {
	if (!func) return;

	int i;
	for (i = 0; i < UINT8_MAX; i++) {
		ish_KVPair *pair;
		for (pair = map->buckets[i]; pair != NULL; pair = pair->next)
			func(pair->key, pair->value, probe);
	}
}

#define ish_MapForPairs(key, value) ish_MapProbePairs(key, value, NULL)

#undef UINT128_LENGTH

int printpairs(char *key, void *value, void *probe) {
	printf("'%s':'%p'\n", key, value);
	return 0;
}

int main(int argc, char *argv[]) {
	ish_Map *map = calloc(1, sizeof(ish_Map));
	ish_MapSet(map, "hello, world", NULL);
	ish_MapSet(map, "myke my boy", NULL);
	ish_MapSet(map, "hello, world", NULL);
	ish_MapSet(map, "lol wut", NULL);
	ish_MapForPairs(map, printpairs);
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
