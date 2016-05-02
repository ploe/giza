#ifndef _GIZA_TYPES
#define _GIZA_TYPES

#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*	Constants	*/
enum {
	giz_CONTINUE = -1,
	giz_BREAK

};

/* 	utils.c		*/
int giz_EachGlob(char *pattern, int (*func)(char *));

#endif