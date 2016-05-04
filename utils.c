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


