%{
#include "fb.tab.h"

void other_error();
%}

%%

[0-9]+	{ yylval = atoi(yytext); return (NUMBER); }
"+" 	{ return (PLUS); }
"-" 	{ return (MINUS); }
"*" 	{ return (STAR); }
"/" 	{ return (BACKSLASH); }
"^" 	{ return (EXPO); }
"++" 	{ return (INC); }
"--" 	{ return (DEC); }
"(" 	{ return (LBrack); }
")" 	{ return (RBrack); }
";" 	{ return (SEMICOLON); }

[ \t]*	{ }
\n 		{ return (EOL); }
.		{ other_error(); return (OTHER); }

%%	


void other_error() {
	printf("Symbol other than number");
}
