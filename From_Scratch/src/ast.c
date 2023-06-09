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

struct ast *newasgn(struct ast *s, struct ast *v) {
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

struct ast *newIf(int nodetype, struct ast *cond, struct ast *tl, struct ast *elsif, struct ast *el) {
	struct astIf *a = malloc(sizeof(struct astIf));

	a->nodetype = nodetype;
	a->cond = cond;
	a->tl = tl;
	a->elif = elsif;
	a->el = el;
	return (struct ast *)a;
}

struct ast *newWh(int nodetype, struct ast *cond, struct ast *tl) {
	struct astWh *a = malloc(sizeof(struct astWh));

	a->nodetype = nodetype;
	a->cond = cond;
	a->tl = tl;
	return (struct ast *)a;
}

struct ast *newFor(int nodetype, struct ast *name, struct ast *exp1, struct ast *exp2, struct ast *tl) {
	struct astFor *a = malloc(sizeof(struct astFor));

	a->nodetype = nodetype;
	a->name = name;
	a->exp1 = exp1;
	a->exp2 = exp2;
	a->tl = tl;
	return (struct ast *)a;
}

struct ast *newStr(int nodetype, char *s) {
	struct astStr *a = malloc(sizeof(struct astStr));
	a->str = malloc(sizeof(char) * 100);

	a->nodetype = nodetype;
	a->str = strdup(s);
	return (struct ast *)a;
}
