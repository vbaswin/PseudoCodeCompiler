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
[0-9]+"."?[0-9]+ 	{ yylval.d = atof(yytext); return NUM; }

([a-zA-Z]+[_a-zA-Z0-9]*) 	{ yylval.s = lookup(yytext); return NAME; }

[ \t]+	{ }
\n 		{ return EOL; }
.	{ yyerror("Mystery character %c\n", *yytext); }

<<EOF>>	{ return EOF; }

%%
