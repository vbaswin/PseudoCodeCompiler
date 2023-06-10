#include "../inc/lextab.h"
#include "../inc/cwd.h"

int ind = 0;
struct table lextab[10000];

void insert_to_lextab(char *name, char *type) {
	strcpy(lextab[ind].name, strdup(name));
	strcpy(lextab[ind].type, strdup(type));
	ind++;
}

void print_lextab() {
	char completePath[500];
	getCompletePath(completePath, "/output/lextab.txt");

	FILE *fp = fopen(completePath, "a+");

	if (fp == NULL) {
		printf("Error opening the file.\n");
		return;
	}
	int max_sz = -100000, sz;

	for (int i = 0; lextab[i].name != NULL && i < ind; i++) {
		sz = strlen(lextab[i].name);
		if (sz > max_sz)
			max_sz = sz;
	}

	fprintf(fp, "****LEXICAL ANALYSIS****\n\n");
	for (int i = 0; lextab[i].name != NULL && i < ind; i++) {
		fprintf(fp, "%-*s%s\n", max_sz + 4, lextab[i].name, lextab[i].type);
	}

	fprintf(fp, "\n\n");
	fclose(fp);
}
