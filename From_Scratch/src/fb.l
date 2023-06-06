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


 /* comparison ops */
">"     { yylval.fn = 1; return CMP; }
"<"     { yylval.fn = 2; return CMP; }
"<>"    { yylval.fn = 3; return CMP; }
"=="    { yylval.fn = 4; return CMP; }
">="    { yylval.fn = 5; return CMP; }
"<="    { yylval.fn = 6; return CMP; }

	/* number */
[0-9]+("."?[0-9]+)? 	{ yylval.d = atof(yytext); return NUM; }

([a-zA-Z]+[_a-zA-Z0-9]*) 	{ yylval.s = lookup(yytext); return NAME; }

[ \t]+
^[ \t]*\n

\n 		{ return EOL; }
.	{ yyerror("Mystery character %c\n", *yytext); }

<<EOF>>	{ return EOF; }

%%
