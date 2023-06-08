#ifndef FB_H
#define FB_H

#include "basic.h"

int yylex();
int yyparse();
void yyerror(const char *s, ...);

extern FILE *yyin;
extern int yylineno;

#endif
