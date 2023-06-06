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

%token IF THEN ELSE END WHILE DO LET 

%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%type <a> exp stmt 

%start prog

%%

prog:
	| prog stmt EOL { printf("> %4.4g\n", eval($2)); displayAst($2,3); }
	;
	
stmt: IF exp THEN stmt END IF { $$ = newIf('I', $2, $4, NULL, NULL);}
	| IF exp THEN stmt ELSE stmt END IF { $$ = newIf('I', $2, $4,NULL, $6);}
    | exp
	;

exp: exp CMP exp 		{ $$ = newcmp($2, $1, $3); }
	| exp '+' exp          	{ $$ = newast('+', $1,$3); }
   | exp '-' exp          	{ $$ = newast('-', $1, $3); }
   | exp '*' exp          	{ $$ = newast('*', $1,$3); }
   | exp '/' exp          	{ $$ = newast('/', $1, $3); }
   | '(' exp ')'          	{ $$ = $2 }
   | '-' exp %prec UMINUS 	{ $$ = newast('M', $2, NULL); }
   | NUM	              	{ $$ = newnum($1); }
   | NAME				  	{ $$ = newref($1); }
   | NAME '=' exp			{ $$ = newasgn($1, $3); }
   ;

%%