%{
#include "../inc/fb.h"
%}

// declare symtab with value as union
// so we can add any type of value 
// not just double or int - optional

/* declare tokens */
%token NUM
%token EOL 

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%start prog

%%

prog:
	| prog exp EOL { printf("> %d\n", $2); }
	;

exp: exp '+' exp          { $$ = $1 + $3 }
   | exp '-' exp          { $$ = $1 - $3 }
   | exp '*' exp          { $$ = $1 * $3 }
   | exp '/' exp          { $$ = $1 / $3 }
   | '(' exp ')'          { $$ = $2; }
   | '-' exp %prec UMINUS { $$ = -$2 }
   | NUM	              { $$ = $1 }
   ;

%%