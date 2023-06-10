%define parse.lac full
%define parse.error detailed

%{
#include "../inc/fb.h"
#include "../inc/symtab.h"
#include "../inc/ast.h"
#include "../inc/cwd.h"

%}

%union {
	struct ast *a;
	double d;
	struct symbol *s;
	int fn;
	char *str;
}

/* declare tokens */
%token <d> NUM
%token <s> NAME
%token <str> STRING

/* flow */
%token IF THEN ELSE END WHILE DO FOR TO 

/* display */
%token PRINT 

%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%type <a> exp stmt stmts

%start prog

%%

prog:
	| prog stmt ';' { Evaluate($2); }
	;
	
stmt: IF exp THEN stmts END IF 				{ $$ = newIf('I', $2, $4, NULL, NULL); }
	| IF exp THEN stmts ELSE stmts END IF 	{ $$ = newIf('I', $2, $4,NULL, $6);}
	| WHILE exp DO stmts END WHILE			{ $$ = newWh('W', $2, $4); }
	| FOR NAME '=' exp TO exp DO stmts END FOR { $$ = newFor('F', newref($2), $4, $6, $8); }
    | exp
	;

stmts: {$$ = NULL; }
	| stmt ',' stmts { if ($3 == NULL)
							$$ = $1;
						else 
							$$ = newast('L', $1, $3);
					}
	| stmt			{ $$ = $1;}


exp: 
	PRINT '(' exp ')' { $$ = newast('P', $3, NULL); }
 | exp CMP exp 		{ $$ = newcmp($2, $1, $3); }
	| exp '+' exp          	{ $$ = newast('+', $1,$3);}
   | exp '-' exp          	{ $$ = newast('-', $1, $3); }
   | exp '*' exp          	{ $$ = newast('*', $1,$3); }
   | exp '/' exp          	{ $$ = newast('/', $1, $3); }
   | '(' exp ')'          	{ $$ = $2; }
   | '-' exp %prec UMINUS 	{ $$ = newast('M', $2, NULL); }
   | NUM	              	{ $$ = newnum($1); }
   | NAME				  	{ $$ = newref($1); }
   | STRING					{ $$ = newStr('S',$1); }
   | NAME '=' exp			{ $$ = newasgn(newref($1), $3); }
   ;

%%
