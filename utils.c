#include "giza.h"

/*	giz_EachGlob - Iterates over all of the paths defined in [pattern] and
	calls [func] on them. */

int giz_EachGlob(char *pattern, int (*func)(char *)) {
	glob_t g;
	int err = glob(pattern, GLOB_MARK | GLOB_TILDE_CHECK, NULL, &g);

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
