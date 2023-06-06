#ifndef SYMTAB_H
#define SYMTAB_H

#include "basic.h"

struct symbol {
	char *name;
	struct ref *reflist;
};

struct ref {
	struct ref *next;
	int lineno;
};

extern struct symbol symtab[100];
struct symbol *lookup(char *);

void addref(int, char *);
void printrefs();
void freeSymtab();

extern int cur;

#endif
