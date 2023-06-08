#include "../inc/lextab.h"
#include "../inc/cwd.h"

int ind = 0;
struct table lextab[100];

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

	fprintf(fp, "****LEXICAL ANALYSIS****\n\n");
	for (int i = 0; lextab[i].name != NULL && i < ind; i++) {
		fprintf(fp, "%s\t\t%s\n", lextab[i].name, lextab[i].type);
	}

	fprintf(fp, "\n\n");
	fclose(fp);
}
