#include "hashish.h"

/*	SetHash.
	Uses Murmur3 to convert a [key] in to a new [hash]	*/

static void SetHash(char *key, void *out) {
	MurmurHash3_x64_128(key, strlen(key), getpid(), out);
}

/*	FindPair:
	Finds the KVPair with [key] in [map]	*/
	

static ish_KVPair *FindPair(ish_Map *map, char *key) {
	uint8_t hash[ish_UINT128_LENGTH];
	SetHash(key, hash);

	ish_KVPair *top = map->buckets[hash[0]], *pair;
	for (pair = top; pair != NULL; pair = pair->next) {
		if (memcmp(hash, pair->hash, sizeof(uint8_t) * ish_UINT128_LENGTH) == 0)
			if (strcmp(key, pair->key) == 0) return pair;
	}
	return NULL;
}



static ish_KVPair *NewKVPair(ish_Map *map, char *key) {
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
	if ((pair = FindPair(map, key)) == NULL) pair = NewKVPair(map, key);

	if (pair) {
		pair->value = value;
		pair->destruct = destruct;
		return 1;

	}
	return 0;
}


void ish_MapProbePairs(ish_Map *map, int (*func)(char *, void *, void *), void *probe) {
	if (!func) return;

	int i;
	for (i = 0; i < UINT8_MAX; i++) {
		ish_KVPair *pair;
		for (pair = map->buckets[i]; pair != NULL; pair = pair->next)
			func(pair->key, pair->value, probe);
	}
}


#undef ish_UINT128_LENGTH


