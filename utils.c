#include "giza.h"

/*	giz_EachGlob:

	Iterates over all of the paths defined in [pattern] and
	calls [func] on them.	*/

int giz_EachGlob(char *pattern, int (*func)(char *)) {
	glob_t g;
	int err = glob(pattern, GLOB_MARK | GLOB_TILDE, NULL, &g);

	if (!err && func) {
		int i;
		// func should return giz_CONTINUE or giz_BREAK
		for (i = 0; i < g.gl_pathc; i++) {
			if(func(g.gl_pathv[i])) continue;
			else break;
		}
	}

	globfree(&g);

	return err;
}

/*	giz_AbsorbFile:

	Reads all the contents of a file descriptor, allocates the spaces 
	for it and dumps it in a char array and returns the address of it.

	I swithered between using an fd or a stream. I went for an fd as it's 
	more Unix aware.	*/

#define BUFFER_SIZE 1024
char *giz_AbsorbFile(int fd) {
	int n, len = 0, size = BUFFER_SIZE;
	char buffer[BUFFER_SIZE];
	char *str = calloc(BUFFER_SIZE, sizeof(char)*size);

	
	while ((n = read(fd, buffer, BUFFER_SIZE)) > 0) {
		// Double the size of str if we've filled it up with char.
		if ((len + n) > size) {
//			printf("(%d + %d) > %d\n", len, n, size);
//			printf("new size: %d\n", size*2);
			size *= 2;
			char *tmp = realloc(str, sizeof(char) * size);
			if (tmp) str = tmp;
			else {
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


/*	giz_ValidJson:
	
	Uses murp to ensure the [json] is valid. If it is the value will 
	be non-zero.	*/

static mp_Atomizer validate_json(mp_Atom atom, void *p) {
	int *success = p;
	if (atom.type == mp_DONE) *success = 1;
	else if(mp_IS_ERROR(atom.type)) return mp_BREAK;
	return mp_CONTINUE;
}

int giz_ValidJson(char *json) {
	int success = 0;
	mp_Probe(json, validate_json, &success);
	return success;

}
