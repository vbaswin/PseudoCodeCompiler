#include "../inc/ast.h"

struct ast *newast(int nodetype, struct ast *l, struct ast *r) {
	struct ast *a = malloc(sizeof(struct ast));

	a->nodetype = nodetype;
	a->l = l;
	a->r = r;
	return a;
}

struct ast *newnum(double d) {
	struct numval *a = malloc(sizeof(struct numval));

	a->nodetype = 'K';
	a->number = d;
	return (struct ast *)a;
}

struct ast *newasgn(struct symbol *s, struct ast *v) {
	struct symasgn *a = malloc(sizeof(struct symasgn));

	a->nodetype = '=';
	a->s = s;
	a->v = v;
	return (struct ast *)a;
}

struct ast *newref(struct symbol *s) {
	struct symref *a = malloc(sizeof(struct symref));

	a->nodetype = 'N';
	a->s = s;
	return (struct ast *)a;
}

struct ast *
newcmp(int cmptype, struct ast *l, struct ast *r) {
	struct ast *a = malloc(sizeof(struct ast));

	a->nodetype = '0' + cmptype;
	a->l = l;
	a->r = r;
	return a;
}
