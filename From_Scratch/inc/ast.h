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

struct astIf {
	int nodetype;	  /* type I or W */
	struct ast *cond; /* condition */
	struct ast *tl;	  /* then or do list */
	struct ast *elif; /* for else if tree */
	struct ast *el;	  /* optional else list */
};

struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newref(struct symbol *s);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newIf(int nodetype, struct ast *cond, struct ast *tl, struct ast *elsif, struct ast *el);

#endif
