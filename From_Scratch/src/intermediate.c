#include "../inc/intermediate.h"

int reg = 1;
int lab = 1;

type *newType() {
	type *newReg = malloc(sizeof(type));
	newReg->str = malloc(sizeof(char) * 100);
	return newReg;
}

type *newRegPrint() {
	type *newReg = newType();

	newReg->reg = 1;
	sprintf(newReg->str, "r%d", reg++);
	newReg->num = 0;
	printf("%s = ", newReg->str);

	return newReg;
}

label *newLabPrint() {
	label *newLab = malloc(sizeof(label));
	newLab->name = malloc(sizeof(char) * 100);

	sprintf(newLab->name, "l%d", lab++);
	return newLab;
}

type *newReg_printCode(type *first, type *second, char *op) {
	type *newReg, *temp;

	if (!strcmp(op, "=2")) {
		if (second->reg)
			return second;

		temp = newRegPrint();
		if (second->str)
			printf("%s", second->str);
		else
			printf("%d", second->num);
		printf("\n");
		return temp;
	} else if (!strcmp(op, "=")) {
		newReg = newType();
		newReg->num = 0;
		newReg->reg = 0;
		newReg->str = strdup(((struct symasgn *)first)->s->name);
		printf("%s = %s\n", newReg->str, newReg_printCode(first, second, "=2")->str);
		return newReg;
	}

	newReg = newRegPrint();

	if (first->reg || first->str)
		printf("%s", first->str);
	else
		printf("%d", first->num);
	printf(" %s ", op);
	if (second->reg || second->str)
		printf("%s", second->str);
	else
		printf("%d", second->num);
	printf("\n");
	return newReg;
}

type *intermediateCode(struct ast *a) {
	type *left, *right, *newReg = newType();
	newReg->num = 0;
	newReg->reg = 0;
	newReg->str = 0;

	switch (a->nodetype) {
	/* constant */
	case 'K':
		newReg->num = ((struct numval *)a)->number;
		newReg->reg = 0;
		newReg->str = 0;
		return newReg;

	/* name reference */
	case 'N':
		newReg->num = 0;
		newReg->reg = 0;
		newReg->str = strdup(((struct symref *)a)->s->name);
		return newReg;

		// 	/* expressions */

	case '=':
		right = intermediateCode(((struct symasgn *)a)->v);
		return newReg_printCode((type *)a, right, "=");
	case '+':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newReg_printCode(left, right, "+");
	case '-':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newReg_printCode(left, right, "-");
	case '*':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newReg_printCode(left, right, "*");
	case '/':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newReg_printCode(left, right, "/");
	case '1':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newReg_printCode(left, right, ">");
	case '2':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newReg_printCode(left, right, "<");
	case '3':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newReg_printCode(left, right, "<>");
	case '4':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newReg_printCode(left, right, "==");
	case '5':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newReg_printCode(left, right, ">=");
	case '6':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newReg_printCode(left, right, "<=");


		// case 'M':
		// 	printf("unop %c\n", a->nodetype);
		// 	displayAst(a->l, level + 1);
		// 	return;

	case 'I':
		type *cond = newType(), *list_if = newType(), *list_else = newType();
		label *lab1, *lab2;

		if (((struct astIf *)a)->cond) {
			cond = intermediateCode(((struct astIf *)a)->cond);
			// intermediateCode
			// if num or var returned code here
		}
		lab1 = newLabPrint();
		printf("if %s goto %s\n", cond->str, lab1->name);
		if (((struct astIf *)a)->el) {
			cond = intermediateCode(((struct astIf *)a)->el);
		}
		lab2 = newLabPrint();
		printf("goto %s\n%s:\n", lab2->name, lab1->name);
		if (((struct astIf *)a)->tl) {
			cond = intermediateCode(((struct astIf *)a)->tl);
		}
		printf("%s\n", lab2->name);

		// if (((struct astIf *)a)->el) {
		// 	printf("%*s", 4 * level, "");
		// 	printf("else\n");
		// 	displayAst(((struct astIf *)a)->el, level + 1);
		// }
		return newReg;

	case 'L':
		intermediateCode(a->l);
		intermediateCode(a->r);
		return newReg;


		// case 'W':
		// 	printf("While\n");
		// 	displayAst(((struct astWh *)a)->cond, level + 1);
		// 	if (((struct astWh *)a)->tl)
		// 		displayAst(((struct astWh *)a)->tl, level + 1);
		// 	return;

		// case 'F':
		// 	printf("For\n");
		// 	displayAst(newasgn(((struct symref *)a)->s, ((struct astFor *)a)->exp1), level + 1);
		// 	displayAst(((struct astFor *)a)->exp1, level + 1);
		// 	displayAst(((struct astFor *)a)->exp2, level + 1);
		// 	if (((struct astFor *)a)->tl)
		// 		displayAst(((struct astFor *)a)->tl, level + 1);
		// 	return;

		// default:
		// 	printf("bad %c\n", a->nodetype);
		// 	return;
	}
}
