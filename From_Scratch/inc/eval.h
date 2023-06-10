#ifndef EVAL_H
#define EVAL_H

#include "ast_symtab_headers.h"

void displayAstHandle(struct ast *);
void displayEvalHandle(struct ast *a);
void freeAst(struct ast *a);

#endif
