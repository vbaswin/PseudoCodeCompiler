#include "../inc/fb.h"
#include "../inc/cwd.h"
#include "../inc/eval.h"
#include "../inc/intermediate.h"
#include "../inc/lextab.h"
#include "../inc/symtab.h"

void yyerror(const char *s, ...) {
	va_list ap;
	va_start(ap, s);

	fprintf(stderr, "%d: error: ", yylineno);
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}

void Evaluate(struct ast *a) {
	displayAstHandle(a);
	intermediateCodeHandle(a);
	displayEvalHandle(a);

	freeAst(a);
}

int main(int argc, char **argv) {
	if (argc > 1) {
		if (!(yyin = fopen(argv[1], "r"))) {
			perror(argv[1]);
			return (1);
		}
	}
	yyparse();
	print_lextab();
	printrefs();


	return 0;
}
