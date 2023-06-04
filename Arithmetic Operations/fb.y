%{
#include <stdio.h>
#include <math.h>

int yylex();
int yyerror(char *s);
%}

%token PLUS MINUS STAR BACKSLASH EXPO UMINUS INC DEC 
%token NUMBER 
%token EOL LBrack RBrack SEMICOLON OTHER

%%

calc:
	| calc exp EOL 		{ printf("Result = %d\n", $2)}
	;

exp:
		exp PLUS factor 			{ $$ = $1 + $3; }
	| 	exp MINUS factor			{ $$ = $1 - $3; }
	| 	factor
	;

factor:
		factor STAR group 			{ $$ = $1 * $3; }
	| 	factor BACKSLASH group 		{ $$ = $1 / $3; }
	| 	group
	;

group: 
		inc_dec
	| 	inc_dec EXPO group 			{ $$ = pow($1, $3)}

inc_dec:
		INC neg 				{ $$ = $2 + 1}
	|	DEC neg					{ $$ = $2 - 1}
	| 	neg

neg:	
		MINUS term %prec UMINUS		{$$ = -$2}
	|	term

term:
		NUMBER
	| 	LBrack exp RBrack 			{ $$ = $2 }
	;
%%

int yyerror(char *s) {
	fprintf(stderr, "Parser error: %s\n", s);
}

int main() {
	yyparse();
	return 0;
}