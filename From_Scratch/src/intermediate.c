#include "../inc/intermediate.h"

int reg = 1;

type *newRegister() {
	type *newReg = malloc(sizeof(type));
	newReg->str = malloc(sizeof(char) * 100);
	return newReg;
}

type *newRegPrint() {
	type *newReg = newRegister();

	newReg->reg = 1;
	sprintf(newReg->str, "r%d", reg++);
	newReg->num = 0;
	printf("%s = ", newReg->str);

	return newReg;
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
		newReg = newRegister();
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
	type *left, *right, *newReg = newRegister();

	// printf("%*s", 4 * level, ""); /* indent to this level */

	// if (!a) {
	// 	printf("NULL\n");
	// 	return;
	// }

	switch (a->nodetype) {
	// 	/* constant */
	case 'K':
		newReg->num = ((struct numval *)a)->number;
		newReg->reg = 0;
		newReg->str = 0;
		return newReg;

	// 	/* name reference */
	case 'N':
		// printf("%s", ((struct symref *)a)->s->name);
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
		// case 'L':
		// 	printf("binop L\n");
		// 	displayAst(a->l, level + 1);
		// 	displayAst(a->r, level + 1);
		// 	return;

		// case 'M':
		// 	printf("unop %c\n", a->nodetype);
		// 	displayAst(a->l, level + 1);
		// 	return;

		// case 'I':
		// 	printf("if\n");
		// 	if (((struct astIf *)a)->cond)
		// 		displayAst(((struct astIf *)a)->cond, level + 1);
		// 	if (((struct astIf *)a)->tl)
		// 		displayAst(((struct astIf *)a)->tl, level + 1);
		// 	if (((struct astIf *)a)->elif)
		// 		displayAst(((struct astIf *)a)->elif, level + 1);
		// 	if (((struct astIf *)a)->el) {
		// 		printf("%*s", 4 * level, ""); /* indent to this level */
		// 		printf("else\n");
		// 		displayAst(((struct astIf *)a)->el, level + 1);
	}

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
