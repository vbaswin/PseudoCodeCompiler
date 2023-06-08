%option noyywrap yylineno

%{

#include "../inc/fb.h"
#include "../inc/symtab.h"
#include "../obj/fb.tab.h"

%}

%%

	/* single character operators */
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

 /* comparison operators */
">"     { yylval.fn = 1; return CMP; }
"<"     { yylval.fn = 2; return CMP; }
"<>"    { yylval.fn = 3; return CMP; }
"=="    { yylval.fn = 4; return CMP; }
">="    { yylval.fn = 5; return CMP; }
"<="    { yylval.fn = 6; return CMP; }

	 /* keywords */
"if"    { return IF; }
"then"  { return THEN; }
"else"  { return ELSE; }
"end"	{ return END; }
"while" { return WHILE; }
"do"    { return DO; }
"for" { return FOR; }
"to"	{ return TO; }


	/* number */
[0-9]+("."?[0-9]+)? 	{ yylval.d = atof(yytext); return NUM; }

	/* name */
([a-zA-Z]+[_a-zA-Z0-9]*) 	{ yylval.s = lookup(yytext); return NAME; }

[ \t]+
\n

.	{ yyerror("Mystery character %c\n", *yytext); }

<<EOF>>	{ return EOF; }

%%
