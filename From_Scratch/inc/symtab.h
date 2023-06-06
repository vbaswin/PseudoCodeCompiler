#ifndef SYMTAB_H
#define SYMTAB_H

#include "basic.h"

/* symbol table */
struct symbol { /* a variable name */
	char *name;
	double value;
};

extern struct symbol symtab[100];
struct symbol *lookup(char *);
struct symbol *newassign(char *, double);

void addref(int, char *);
void printrefs();
void freeSymtab();


extern int cur;

#endif
