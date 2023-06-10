#ifndef LEXTAB_H
#define LEXTAB_H
#include "basic.h"

struct table {
	char name[100];
	char type[100];
};

extern struct table lextab[10000];
void insert_to_lextab(char *, char *);
void print_lextab();
extern int ind;
#endif
