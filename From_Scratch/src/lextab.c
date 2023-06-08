#include "../inc/lextab.h"
int ind = 0;
struct table lextab[100];

void insert_to_lextab(char *name, char *type) {
	strcpy(lextab[ind].name, strdup(name));
	strcpy(lextab[ind].type, strdup(type));
	ind++;
}

void print_lextab() {
	printf("\n****LEXICAL ANALYSIS****\n\n");
	for (int i = 0; lextab[i].name != NULL && i < ind; i++) {
		printf("%s\t\t%s\n", lextab[i].name, lextab[i].type);
	}
}
