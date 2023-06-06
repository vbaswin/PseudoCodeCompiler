%{
#include "../inc/fb.h"
#include "../inc/symtab.h"
#include "../inc/ast.h"
#include "../inc/eval.h"
%}

%union {
	struct ast *a;
	double d;
	struct symbol *s;
	int fn;
}

/* declare tokens */
%token <d> NUM
%token <s> NAME
%token EOL 

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%type <a> exp 

%start prog

%%

prog:
	| prog exp EOL { printf("> %4.4g\n", eval($2)); }
	;

exp: exp '+' exp          	{ $$ = newast('+', $1,$3); }
   | exp '-' exp          	{ $$ = newast('-', $1, $3); }
   | exp '*' exp          	{ $$ = newast('*', $1,$3); }
   | exp '/' exp          	{ $$ = newast('/', $1, $3); }
   | '(' exp ')'          	{ $$ = $2 }
   | '-' exp %prec UMINUS 	{ $$ = newast('M', $2, NULL); }
   | NUM	              	{ $$ = newnum($1); }
   | NAME				  	{ $$ = newref($1); }
   /* | NAME '=' exp			{ $$ = newasgn($1, $3); } */
   ;

%%