#ifndef INTERMEDIATE_H
#define INTERMEDIATE_H

#include "ast_symtab_headers.h"

typedef struct TYPE {
	int num;
	char *str;
	int reg;
} type;

type *intermediateCode(struct ast *a);

#endif
