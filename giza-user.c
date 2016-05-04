#include "giza.h"

int main(int argc, char *argv[]) {
	puts("giza user executed!");
	char *json = giz_AbsorbFile(STDIN_FILENO);
	printf("%s", json);
	free(json);
	return 0;
}
