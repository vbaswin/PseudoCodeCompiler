#ifndef LEXTAB_H
#define LEXTAB_H
#include "basic.h"

struct table {
	char name[25];
	char type[25];
};

extern struct table lextab[100];
void insert_to_lextab(char *, char *);
void print_lextab();
extern int ind;
#endif
