%option noyywrap yylineno

%{

#include "../inc/fb.h"
#include "../inc/symtab.h"
#include "../inc/lextab.h"
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
")"     { insert_to_lextab(yytext, "single char operator"); return yytext[0]; }

 /* comparison operators */
">"     { insert_to_lextab(yytext, "comparison operator"); yylval.fn = 1; return CMP; }
"<"     { insert_to_lextab(yytext, "comparison operator"); yylval.fn = 2; return CMP; }
"<>"    { insert_to_lextab(yytext, "comparison operator"); yylval.fn = 3; return CMP; }
"=="    { insert_to_lextab(yytext, "comparison operator"); yylval.fn = 4; return CMP; }
">="    { insert_to_lextab(yytext, "comparison operator"); yylval.fn = 5; return CMP; }
"<="    { insert_to_lextab(yytext, "comparison operator"); yylval.fn = 6; return CMP; }

	 /* keywords */
"if"    { insert_to_lextab(yytext, "keyword"); return IF; }
"then"  { insert_to_lextab(yytext, "keyword"); return THEN; }
"else"  { insert_to_lextab(yytext, "keyword"); return ELSE; }
"end"	{ insert_to_lextab(yytext, "keyword"); return END; }
"while" { insert_to_lextab(yytext, "keyword"); return WHILE; }
"do"    { insert_to_lextab(yytext, "keyword"); return DO; }
"for" { insert_to_lextab(yytext, "keyword"); return FOR; }
"to"	{ insert_to_lextab(yytext, "keyword"); return TO; }
"print" { return PRINT; }
 

	/* string */
\"([ a-zA-Z0-9!]+)\" { yylval.str = yytext; insert_to_lextab(yylval.str,"string"); return STRING; }

	/* number */
[0-9]+("."?[0-9]+)? 	{ insert_to_lextab(yytext, "number"); yylval.d = atof(yytext); return NUM; }

	/* name */
([a-zA-Z]+[_a-zA-Z0-9]*) 	{ insert_to_lextab(yytext, "identifier"); yylval.s = lookup(yytext); return NAME; }

[ \t]+
\n

.	{ yyerror("Mystery character %c\n", *yytext); }

<<EOF>>	{ return EOF; }

%%
