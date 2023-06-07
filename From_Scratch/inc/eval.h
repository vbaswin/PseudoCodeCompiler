#ifndef EVAL_H
#define EVAL_H

#include "ast_symtab_headers.h"

double eval(struct ast *a);
void displayAst(struct ast *a, int level);

#endif
