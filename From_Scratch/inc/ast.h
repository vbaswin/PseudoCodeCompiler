#ifndef AST_H
#define AST_H

#include "basic.h"

struct ast {
	int nodetype;
	struct ast *l;
	struct ast *r;
};

struct numval {
	int nodetype;
	double number;
};

struct symasgn {
	int nodetype;
	struct ast *s;
	struct ast *v;
};

struct symref {
	int nodetype;
	struct symbol *s;
};

struct astIf {
	int nodetype;
	struct ast *cond;
	struct ast *tl;
	struct ast *elif;
	struct ast *el;
};

struct astWh {
	int nodetype;
	struct ast *cond;
	struct ast *tl;
};

struct astFor {
	int nodetype;
	struct ast *name;
	struct ast *exp1;
	struct ast *exp2;
	struct ast *tl;
};

struct astStr {
	int nodetype;
	char *str;
};

struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newasgn(struct ast *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newref(struct symbol *s);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newIf(int nodetype, struct ast *cond, struct ast *tl, struct ast *elsif, struct ast *el);
struct ast *newWh(int nodetype, struct ast *cond, struct ast *tl);
struct ast *newFor(int nodetype, struct ast *name, struct ast *exp1, struct ast *exp2, struct ast *tl);
struct ast *newStr(int nodetype, char *s);


#endif
