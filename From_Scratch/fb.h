#ifndef FB_H
#define FB_H

#include <stdio.h>

int yylex();
int yyparse();
void yyerror(char *s, ...);

extern FILE *yyin;
extern int yylineno;

#endif
