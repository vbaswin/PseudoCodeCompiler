#ifndef FB_H
#define FB_H

#include "ast.h"
#include "basic.h"

int yylex();
int yyparse();
void yyerror(const char *s, ...);

extern FILE *yyin;
extern int yylineno;
void Evaluate(struct ast *);

#endif
