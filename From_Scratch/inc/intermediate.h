#ifndef INTERMEDIATE_H
#define INTERMEDIATE_H

#include "ast_symtab_headers.h"

typedef struct TYPE {
	int num;
	char *str;
	int reg;
} type;

typedef struct LABEL {
	char *name;
} label;

type *intermediateCode(struct ast *a, FILE *fp);
void intermediateCodeHandle(struct ast *a);

#endif
