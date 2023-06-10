#include "../inc/eval.h"
#include "../inc/cwd.h"

char *trimQuotes(char *s);

double eval(struct ast *a, FILE *fp) {
	double v = 0.0;

	switch (a->nodetype) {
	case 'K':
		v = ((struct numval *)a)->number;
		break;

	case 'S':
		break;

	case 'N':
		v = ((struct symref *)a)->s->value;
		break;

	case '=':
		v = ((struct symref *)(((struct symasgn *)a)->s))->s->value =
			eval(((struct symasgn *)a)->v, fp);
		break;

	case '+': v = eval(a->l, fp) + eval(a->r, fp); break;
	case '-': v = eval(a->l, fp) - eval(a->r, fp); break;
	case '*': v = eval(a->l, fp) * eval(a->r, fp); break;
	case '/': v = eval(a->l, fp) / eval(a->r, fp); break;
	case '|': v = fabs(eval(a->l, fp)); break;
	case 'M':
		v = -eval(a->l, fp);
		break;

	case '1': v = (eval(a->l, fp) > eval(a->r, fp)) ? 1 : 0; break;
	case '2': v = (eval(a->l, fp) < eval(a->r, fp)) ? 1 : 0; break;
	case '3': v = (eval(a->l, fp) != eval(a->r, fp)) ? 1 : 0; break;
	case '4': v = (eval(a->l, fp) == eval(a->r, fp)) ? 1 : 0; break;
	case '5': v = (eval(a->l, fp) >= eval(a->r, fp)) ? 1 : 0; break;
	case '6': v = (eval(a->l, fp) <= eval(a->r, fp)) ? 1 : 0; break;

	case 'I':
		if (eval(((struct astIf *)a)->cond, fp) != 0) {
			if (((struct astIf *)a)->tl) {
				v = eval(((struct astIf *)a)->tl, fp);
			} else
				v = 0.0;
		} else {
			if (((struct astIf *)a)->el) {
				v = eval(((struct astIf *)a)->el, fp);
			} else {
				v = 0.0;
			}
		}
		break;

	case 'L':
		eval(a->l, fp);
		v = eval(a->r, fp);
		break;

	case 'W':
		v = 0.0;

		if (((struct astWh *)a)->tl) {
			while (eval(((struct astWh *)a)->cond, fp) != 0) {
				v = eval(((struct astWh *)a)->tl, fp);
			}
		}
		break;

	case 'F':
		v = 0.0;
		((struct symref *)(((struct astFor *)a)->name))->s->value = (int)(eval(((struct astFor *)a)->exp1, fp));

		if (((struct astFor *)a)->tl) {
			for (int i = (int)(eval(((struct astFor *)a)->exp1, fp)); i < (int)(eval(((struct astFor *)a)->exp2, fp)); ++i) {
				v = eval(((struct astFor *)a)->tl, fp);
				++((struct symref *)((struct astFor *)a)->name)->s->value;
			}
		}
		break;

	case 'P':
		switch (a->l->nodetype) {
		case 'K':
			fprintf(fp, "\t%4.4g\n", ((struct numval *)a->l)->number);
			break;
		case 'N':
			fprintf(fp, "\t%4.4g\n", ((struct symref *)a->l)->s->value);
			break;
		case 'S':
			fprintf(fp, "\t%s\n", trimQuotes(strdup(((struct astStr *)a->l)->str)));
			break;
		}
		break;

	default: fprintf(fp, "\tinternal error: bad node %c\n", a->nodetype);
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
	case 'K':
		fprintf(fp, "number %4.4g\n", ((struct numval *)a)->number);
		break;

	case 'N':
		fprintf(fp, "ref %s\n", ((struct symref *)a)->s->name);
		break;

	case '=':
		fprintf(fp, "=\n");
		displayAst(((struct symasgn *)a)->s, level + 1, fp);
		displayAst(((struct symasgn *)a)->v, level + 1, fp);
		return;

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
		displayAst(newasgn((((struct astFor *)a)->name), ((struct astFor *)a)->exp1), level + 1, fp);
		displayAst(((struct astFor *)a)->exp1, level + 1, fp);
		displayAst(((struct astFor *)a)->exp2, level + 1, fp);
		if (((struct astFor *)a)->tl)
			displayAst(((struct astFor *)a)->tl, level + 1, fp);
		return;
	case 'P':
		fprintf(fp, "print\n");
		displayAst(a->l, level + 1, fp);
		return;

	case 'S':
		fprintf(fp, "string %s\n", ((struct astStr *)a)->str);
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

void displayEvalHandle(struct ast *a) {
	char completePath[500];
	getCompletePath(completePath, "/output/output.txt");

	FILE *fp = fopen(completePath, "a+");

	if (fp == NULL) {
		printf("Error opening the file.\n");
		return;
	}
	fprintf(fp, ">");
	eval(a, fp);
	fprintf(fp, "\n\n");
	fclose(fp);
}

void freeAst(struct ast *a) {
	if (!a)
		return;

	switch (a->nodetype) {
	case 'K':
	case 'N':
	case 'S':
		break;


	case '=':
		freeAst(((struct symasgn *)a)->s);
		freeAst(((struct symasgn *)a)->v);
		break;

	case '+':
	case '-':
	case '*':
	case '/':
	case 'L':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
		freeAst(a->l);
		freeAst(a->r);
		break;

	case 'M':
		freeAst(a->l);
		break;


	case 'I':
		if (((struct astIf *)a)->cond)
			freeAst(((struct astIf *)a)->cond);
		if (((struct astIf *)a)->tl)
			freeAst(((struct astIf *)a)->tl);
		if (((struct astIf *)a)->elif)
			freeAst(((struct astIf *)a)->elif);
		if (((struct astIf *)a)->el) {
			freeAst(((struct astIf *)a)->el);
		}
		break;

	case 'W':
		freeAst(((struct astWh *)a)->cond);
		if (((struct astWh *)a)->tl)
			freeAst(((struct astWh *)a)->tl);
		break;

	case 'F':
		freeAst(((struct astFor *)a)->name);
		freeAst(((struct astFor *)a)->exp1);
		freeAst(((struct astFor *)a)->exp2);
		if (((struct astFor *)a)->tl)
			freeAst(((struct astFor *)a)->tl);
		break;
	case 'P':
		freeAst(a->l);
		break;

	default:
		break;
	}
	free(a);
}

char *trimQuotes(char *s) {
	char *temp = strdup(s);
	for (int i = 1, j = 0; i < strlen(s) - 1; ++i, ++j)
		s[j] = temp[i];
	s[strlen(temp) - 2] = '\0';

	return s;
}
