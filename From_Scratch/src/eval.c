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

		/* comparisons */
	case '1': v = (eval(a->l) > eval(a->r)) ? 1 : 0; break;
	case '2': v = (eval(a->l) < eval(a->r)) ? 1 : 0; break;
	case '3': v = (eval(a->l) != eval(a->r)) ? 1 : 0; break;
	case '4': v = (eval(a->l) == eval(a->r)) ? 1 : 0; break;
	case '5': v = (eval(a->l) >= eval(a->r)) ? 1 : 0; break;
	case '6': v = (eval(a->l) <= eval(a->r)) ? 1 : 0; break;

	case 'I':
		if (eval(((struct astIf *)a)->cond) != 0) {
			if (((struct astIf *)a)->tl) {
				v = eval(((struct astIf *)a)->tl);
			} else
				v = 0.0;
		} else {
			if (((struct astIf *)a)->el) {
				v = eval(((struct astIf *)a)->el);
			} else {
				v = 0.0;
			}
		}
		break;

	case 'L':
		eval(a->l);
		v = eval(a->r);
		break;

	case 'W':
		v = 0.0; /* a default value */

		if (((struct astWh *)a)->tl) {
			while (eval(((struct astWh *)a)->cond) != 0)
				v = eval(((struct astWh *)a)->tl);
		}
		break; /* last value is value */

	case 'F':
		v = 0.0; /* a default value */

		if (((struct astFor *)a)->tl) {
			for (int i = ((struct astFor *)a)->start; i < ((struct astFor *)a)->end; ++i)
				v = eval(((struct astFor *)a)->tl);
		}
		break;


	default: printf("internal error: bad node %c\n", a->nodetype);
	}
	return v;
}

void displayAst(struct ast *a, int level) {
	printf("%*s", 4 * level, ""); /* indent to this level */

	if (!a) {
		printf("NULL\n");
		return;
	}

	switch (a->nodetype) {
		/* constant */
	case 'K':
		printf("number %4.4g\n", ((struct numval *)a)->number);
		break;

		/* name reference */
	case 'N':
		printf("ref %s\n", ((struct symref *)a)->s->name);
		break;

		/* assignment */
	case '=':
		printf("= %s\n", ((struct symref *)a)->s->name);
		displayAst(((struct symasgn *)a)->v, level + 1);
		return;

		/* expressions */
	case '+':
	case '-':
	case '*':
	case '/':
		printf("binop %c\n", a->nodetype);
		displayAst(a->l, level + 1);
		displayAst(a->r, level + 1);
		return;
	case 'L':
		printf("binop L\n");
		displayAst(a->l, level + 1);
		displayAst(a->r, level + 1);
		return;
	case '1':
		printf("binop >\n");
		displayAst(a->l, level + 1);
		displayAst(a->r, level + 1);
		return;
	case '2':
		printf("binop <\n");
		displayAst(a->l, level + 1);
		displayAst(a->r, level + 1);
		return;
	case '3':
		printf("binop <>\n");
		displayAst(a->l, level + 1);
		displayAst(a->r, level + 1);
		return;
	case '4':
		printf("binop ==\n");
		displayAst(a->l, level + 1);
		displayAst(a->r, level + 1);
		return;
	case '5':
		printf("binop >=\n");
		displayAst(a->l, level + 1);
		displayAst(a->r, level + 1);
		return;
	case '6':
		printf("binop <=");
		displayAst(a->l, level + 1);
		displayAst(a->r, level + 1);
		return;

	case 'M':
		printf("unop %c\n", a->nodetype);
		displayAst(a->l, level + 1);
		return;

	case 'I':
		printf("if\n");
		if (((struct astIf *)a)->cond)
			displayAst(((struct astIf *)a)->cond, level + 1);
		if (((struct astIf *)a)->tl)
			displayAst(((struct astIf *)a)->tl, level + 1);
		if (((struct astIf *)a)->elif)
			displayAst(((struct astIf *)a)->elif, level + 1);
		if (((struct astIf *)a)->el) {
			printf("%*s", 4 * level, ""); /* indent to this level */
			printf("else\n");
			displayAst(((struct astIf *)a)->el, level + 1);
		}
		return;

	case 'W':
		printf("While\n");
		displayAst(((struct astWh *)a)->cond, level + 1);
		if (((struct astWh *)a)->tl)
			displayAst(((struct astWh *)a)->tl, level + 1);
		return;


	default:
		printf("bad %c\n", a->nodetype);
		return;
	}
}
