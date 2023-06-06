#ifndef AST_H
#define AST_H

#include "basic.h"

struct ast {
	int nodetype;
	struct ast *l;
	struct ast *r;
};

struct numval {
	int nodetype; /* type K */
	double number;
};

struct symasgn {
	int nodetype; /* type = */
	struct symbol *s;
	struct ast *v; /* value */
};

struct symref {
	int nodetype; /* type N */
	struct symbol *s;
};

struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newref(struct symbol *s);
double eval(struct ast *a);


#endif
