#include "../inc/eval.h"
#include "../inc/cwd.h"

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
		printf("%4.4g", v);
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
			double val = ((struct symref *)((struct astFor *)a))->s->value;

			for (int i = (int)(eval(((struct astFor *)a)->exp1)); i < (int)(eval(((struct astFor *)a)->exp2)); ++i) {
				v = eval(((struct astFor *)a)->tl);
				++val;
			}
			((struct symref *)a)->s->value = val;
		}
		break;


	default: printf("internal error: bad node %c\n", a->nodetype);
	}
	return v;
}

void displayAst(struct ast *a, int level, FILE *fp) {
	fprintf(fp, "%*s", 4 * level, ""); /* indent to this level */

	if (!a) {
		fprintf(fp, "NULL\n");
		return;
	}

	switch (a->nodetype) {
		/* constant */
	case 'K':
		fprintf(fp, "number %4.4g\n", ((struct numval *)a)->number);
		break;

		/* name reference */
	case 'N':
		fprintf(fp, "ref %s\n", ((struct symref *)a)->s->name);
		break;

		/* assignment */
	case '=':
		fprintf(fp, "%s = \n", ((struct symref *)a)->s->name);
		displayAst(((struct symasgn *)a)->v, level + 1, fp);
		return;

		/* expressions */
	case '+':
	case '-':
	case '*':
	case '/':
		fprintf(fp, "binop %c\n", a->nodetype);
		displayAst(a->l, level + 1, fp);
		displayAst(a->r, level + 1, fp);
		return;
	case 'L':
		fprintf(fp, "binop L\n");
		displayAst(a->l, level + 1, fp);
		displayAst(a->r, level + 1, fp);
		return;
	case '1':
		fprintf(fp, "binop >\n");
		displayAst(a->l, level + 1, fp);
		displayAst(a->r, level + 1, fp);
		return;
	case '2':
		fprintf(fp, "binop <\n");
		displayAst(a->l, level + 1, fp);
		displayAst(a->r, level + 1, fp);
		return;
	case '3':
		fprintf(fp, "binop <>\n");
		displayAst(a->l, level + 1, fp);
		displayAst(a->r, level + 1, fp);
		return;
	case '4':
		fprintf(fp, "binop ==\n");
		displayAst(a->l, level + 1, fp);
		displayAst(a->r, level + 1, fp);
		return;
	case '5':
		fprintf(fp, "binop >=\n");
		displayAst(a->l, level + 1, fp);
		displayAst(a->r, level + 1, fp);
		return;
	case '6':
		fprintf(fp, "binop <=");
		displayAst(a->l, level + 1, fp);
		displayAst(a->r, level + 1, fp);
		return;

	case 'M':
		fprintf(fp, "unop %c\n", a->nodetype);
		displayAst(a->l, level + 1, fp);
		return;

	case 'I':
		fprintf(fp, "if\n");
		if (((struct astIf *)a)->cond)
			displayAst(((struct astIf *)a)->cond, level + 1, fp);
		if (((struct astIf *)a)->tl)
			displayAst(((struct astIf *)a)->tl, level + 1, fp);
		if (((struct astIf *)a)->elif)
			displayAst(((struct astIf *)a)->elif, level + 1, fp);
		if (((struct astIf *)a)->el) {
			fprintf(fp, "%*s", 4 * level, ""); /* indent to this level */
			fprintf(fp, "else\n");
			displayAst(((struct astIf *)a)->el, level + 1, fp);
		}
		return;

	case 'W':
		fprintf(fp, "While\n");
		displayAst(((struct astWh *)a)->cond, level + 1, fp);
		if (((struct astWh *)a)->tl)
			displayAst(((struct astWh *)a)->tl, level + 1, fp);
		return;

	case 'F':
		fprintf(fp, "For\n");
		displayAst(newasgn(((struct symref *)a)->s, ((struct astFor *)a)->exp1), level + 1, fp);
		displayAst(((struct astFor *)a)->exp1, level + 1, fp);
		displayAst(((struct astFor *)a)->exp2, level + 1, fp);
		if (((struct astFor *)a)->tl)
			displayAst(((struct astFor *)a)->tl, level + 1, fp);
		return;

	default:
		fprintf(fp, "bad %c\n", a->nodetype);
		return;
	}
}

void displayAstHandle(struct ast *a) {
	char completePath[500];
	getCompletePath(completePath, "/output/ast.txt");

	FILE *fp = fopen(completePath, "a+");

	if (fp == NULL) {
		printf("Error opening the file.\n");
		return;
	}

	displayAst(a, 0, fp);
	fprintf(fp, "\n\n");
	fclose(fp);
}
