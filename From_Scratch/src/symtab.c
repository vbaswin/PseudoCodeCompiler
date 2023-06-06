#include "../inc/symtab.h"

int cur = 0;
struct symbol symtab[100];

struct symbol *
lookup(char *sym) {
	for (int i = 0; i < cur; ++i) {
		if (!strcmp(symtab[i].name, sym))
			return &symtab[i];
	}

	symtab[cur].name = strdup(sym);
	symtab[cur].value = 0;
	++cur;
	return &symtab[cur - 1];
}

struct symbol *newassign(char *str, double d) {
	struct symbol *sp = lookup(str);
	sp->value = d;
}

void printrefs() {
	printf("\n\nSymtab\n\n");

	struct ref *temp;
	int max_sz = -100000, sz;

	for (int i = 0; i < cur; ++i) {
		sz = strlen(symtab[i].name);
		if (sz > max_sz)
			max_sz = sz;
	}

	for (int i = 0; i < cur; ++i) {
		printf("%-*s", max_sz, symtab[i].name);

		printf("\t->\t%4.4g\n", symtab[i].value);
	}
}

/*
void freeSymtab() {
	for (int i = 0; i < cur; ++i) {
		free(symtab[i].name);

		struct ref *temp = symtab[i].reflist, *prev;
		while (!temp) {
			prev = temp;
			temp = temp->next;
			free(prev);
		}
	}
	cur = 0;
}
*/
