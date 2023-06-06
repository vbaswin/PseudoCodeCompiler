%option noyywrap yylineno

%{

#include "../inc/fb.h"
#include "../inc/symtab.h"
#include "../obj/fb.tab.h"

%}

%%

	/* single character ops */
"+" |
"-" |
"*" |
"/" |
"=" |
"|" |
"," |
";" |
"(" |
")"     { return yytext[0]; }

	/* number */
[0-9]+ 	{ yylval = atoi(yytext); return NUM; }

([a-zA-Z]+[_a-zA-Z0-9]*) 	{ addref(yylineno, yytext); }

[ \t]*	{ }
\n 		{ return EOL; }
.	{ yyerror("Mystery character %c\n", *yytext); }

<<EOF>>	{ return EOF; }

%%
