#include "giza.h"
#include "murp.h"

mp_Atomizer print_atom(mp_Atom atom, void *p) {
	printf("Atom {\n\tDatatype:\t%d\n\tContainer:\t%d\n", atom.type, atom.container);
	if(atom.key.start) printf("\tKey:\t%.*s\n", atom.key.len, atom.key.start);
	printf("\tValue:\t%.*s\n", atom.value.len, atom.value.start);
	puts("\n}\n");
	
	if (mp_IS_ERROR(atom.type)) return mp_BREAK;
	if ((atom.type == mp_OBJECT) || (atom.type == mp_ARRAY)) {
		mp_AtomizeSlice(&atom.value, print_atom);
	}
	return mp_CONTINUE;
}

int main(int argc, char *argv[]) {
	puts("giza user executed!");
	char *json = giz_AbsorbFile(STDIN_FILENO);
	puts("absorb complete");
	mp_Atomize(json, print_atom);
	puts("atomize complete");
//	printf("%s", json);
	free(json);
	return 0;
}
