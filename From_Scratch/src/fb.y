%define parse.lac full
%define parse.error detailed

%{
#include "../inc/fb.h"
#include "../inc/symtab.h"
#include "../inc/ast.h"
#include "../inc/eval.h"
#include "../inc/intermediate.h"
#include "../inc/cwd.h"

void writeOutput(double);
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

%token IF THEN ELSE END WHILE DO FOR TO PRINT

%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%type <a> exp stmt stmts

%start prog

%%

prog:
	| prog stmt ';' { displayAstHandle($2); intermediateCodeHandle($2); displayEvalHandle($2);}
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
   | NAME '=' exp			{ $$ = newasgn(newref($1), $3); }
   ;

%%

void writeOutput(double d) {
	char completePath[500];
	getCompletePath(completePath, "/output/output.txt");

	FILE *fp = fopen(completePath, "a+");

	if (fp == NULL) {
		printf("Error opening the file.\n");
		return;
	}

	fprintf(fp, "> %4.6g\n", d); 
	fclose(fp);
}