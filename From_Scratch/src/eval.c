#include "../inc/eval.h"

double eval(struct ast *a) {
	double v;

	switch (a->nodetype) {
		/* constant */
	case 'K':
		v = ((struct numval *)a)->number;
		break;

		/* name reference */
	case 'N':
		v = ((struct symref *)a)->s->value;
		break;

		/* assignment */
	case '=':
		v = ((struct symasgn *)a)->s->value =
			eval(((struct symasgn *)a)->v);
		break;

		/* expressions */
	case '+': v = eval(a->l) + eval(a->r); break;
	case '-': v = eval(a->l) - eval(a->r); break;
	case '*': v = eval(a->l) * eval(a->r); break;
	case '/': v = eval(a->l) / eval(a->r); break;
	case '|': v = fabs(eval(a->l)); break;
	case 'M':
		v = -eval(a->l);
		break;

	default: printf("internal error: bad node %c\n", a->nodetype);
	}
	return v;
}
