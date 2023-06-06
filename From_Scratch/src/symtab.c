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
	symtab[cur].reflist = 0;
	++cur;
	return &symtab[cur - 1];
}

void addref(int lineno, char *word) {
	struct symbol *sp = lookup(word);

	struct ref *r = (struct ref *)malloc(sizeof(struct ref));
	r->lineno = lineno;
	r->next = 0;

	if (!sp->reflist) {
		sp->reflist = r;
	} else {
		struct ref *temp;
		for (temp = sp->reflist; temp->next != NULL; temp = temp->next)
			;
		temp->next = r;
	}
}

void printrefs() {
	struct ref *temp;
	int max_sz = -100000, sz;

	for (int i = 0; i < cur; ++i) {
		sz = strlen(symtab[i].name);
		if (sz > max_sz)
			max_sz = sz;
	}

	for (int i = 0; i < cur; ++i) {
		printf("%-*s", max_sz, symtab[i].name);

		printf("\t->");

		for (temp = symtab[i].reflist; temp != NULL; temp = temp->next)
			printf(" %d", temp->lineno);

		printf("\n");
	}
}

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
