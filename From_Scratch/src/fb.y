%{
#include "../inc/fb.h"
#include "../inc/symtab.h"
#include "../inc/ast.h"
#include "../inc/eval.h"
#include "../inc/intermediate.h"
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

%token IF THEN ELSE END WHILE DO FOR TO

%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%type <a> exp stmt stmts

%start prog

%%

prog:
	| prog stmt ';' { printf("> %4.4g\n", eval($2)); displayAst($2,3); intermediateCode($2); printf("\n");}
	;
	
stmt: IF exp THEN stmts END IF 				{ $$ = newIf('I', $2, $4, NULL, NULL); }
	| IF exp THEN stmts ELSE stmts END IF 	{ $$ = newIf('I', $2, $4,NULL, $6);}
	| WHILE exp DO stmts END WHILE			{ $$ = newWh('W', $2, $4)}
	| FOR NAME '=' exp TO exp DO stmts END FOR { $$ = newFor('F', (struct ast *)$2, $4, $6, $8); }
    | exp
	;

stmts: {$$ = NULL; }
	| stmt ',' stmts { if ($3 == NULL)
							$$ = $1;
						else 
							$$ = newast('L', $1, $3);
					}
	| stmt			{ $$ = $1;}


exp: exp CMP exp 		{ $$ = newcmp($2, $1, $3); }
	| exp '+' exp          	{ $$ = newast('+', $1,$3);}
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