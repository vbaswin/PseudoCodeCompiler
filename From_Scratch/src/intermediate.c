#include "../inc/intermediate.h"

int reg = 1;
int lab = 1;

type *newTypeCreate() {
	type *newType = malloc(sizeof(type));
	newType->str = malloc(sizeof(char) * 100);
	return newType;
}

type *newTypePrint() {
	type *newType = newTypeCreate();

	newType->reg = 1;
	sprintf(newType->str, "r%d", reg++);
	newType->num = 0;
	printf("%s = ", newType->str);

	return newType;
}

label *newLabPrint() {
	label *newLab = malloc(sizeof(label));
	newLab->name = malloc(sizeof(char) * 100);

	sprintf(newLab->name, "l%d", lab++);
	return newLab;
}

type *newType_printCode(type *first, type *second, char *op) {
	type *newType, *temp;

	if (!strcmp(op, "=2")) {
		if (second->reg)
			return second;

		temp = newTypePrint();
		if (second->str)
			printf("%s", second->str);
		else
			printf("%d", second->num);
		printf("\n");
		return temp;
	} else if (!strcmp(op, "=")) {
		newType = newTypeCreate();
		newType->num = 0;
		newType->reg = 0;
		newType->str = strdup(((struct symasgn *)first)->s->name);
		printf("%s = %s\n", newType->str, newType_printCode(first, second, "=2")->str);
		return newType;
	}

	newType = newTypePrint();

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
	return newType;
}

type *intermediateCode(struct ast *a) {
	type *left, *right, *newType = newTypeCreate();
	newType->num = 0;
	newType->reg = 0;
	newType->str = 0;

	switch (a->nodetype) {
	/* constant */
	case 'K':
		newType->num = ((struct numval *)a)->number;
		newType->reg = 0;
		newType->str = 0;
		return newType;

	/* name reference */
	case 'N':
		newType->num = 0;
		newType->reg = 0;
		newType->str = strdup(((struct symref *)a)->s->name);
		return newType;

		// 	/* expressions */

	case '=':
		right = intermediateCode(((struct symasgn *)a)->v);
		return newType_printCode((type *)a, right, "=");

	case '+':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newType_printCode(left, right, "+");
	case '-':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newType_printCode(left, right, "-");
	case '*':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newType_printCode(left, right, "*");
	case '/':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newType_printCode(left, right, "/");
	case '1':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newType_printCode(left, right, ">");
	case '2':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newType_printCode(left, right, "<");
	case '3':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newType_printCode(left, right, "<>");
	case '4':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newType_printCode(left, right, "==");
	case '5':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newType_printCode(left, right, ">=");
	case '6':
		left = intermediateCode(a->l);
		right = intermediateCode(a->r);
		return newType_printCode(left, right, "<=");


		// case 'M':
		// 	printf("unop %c\n", a->nodetype);
		// 	displayAst(a->l, level + 1);
		// 	return;

	case 'I':
		type *cond = newTypeCreate();
		label *lab1 = newLabPrint(), *lab2 = newLabPrint();

		if (((struct astIf *)a)->cond)
			cond = intermediateCode(((struct astIf *)a)->cond);

		printf("if ");
		if (cond->reg || cond->str)
			printf("%s", cond->str);
		else
			printf("%d", cond->num);

		printf(" goto %s\n", lab1->name);
		if (((struct astIf *)a)->el) {
			intermediateCode(((struct astIf *)a)->el);
		}
		printf("goto %s\n%s:\n", lab2->name, lab1->name);
		if (((struct astIf *)a)->tl) {
			intermediateCode(((struct astIf *)a)->tl);
		}
		printf("%s\n", lab2->name);

		// if (((struct astIf *)a)->el) {
		// 	printf("%*s", 4 * level, "");
		// 	printf("else\n");
		// 	displayAst(((struct astIf *)a)->el, level + 1);
		// }
		return newType;

	case 'L':
		intermediateCode(a->l);
		intermediateCode(a->r);
		return newType;


	case 'W':
		cond = newTypeCreate();
		lab1 = newLabPrint();
		lab2 = newLabPrint();
		label *lab3 = newLabPrint();

		printf("%s:\n", lab1->name);
		if (((struct astWh *)a)->cond)
			cond = intermediateCode(((struct astWh *)a)->cond);

		printf("if ");
		if (cond->reg || cond->str)
			printf("%s", cond->str);
		else
			printf("%d", cond->num);

		printf(" goto %s\ngoto %s\n%s:\n", lab2->name, lab3->name, lab2->name);
		if (((struct astWh *)a)->tl) {
			intermediateCode(((struct astIf *)a)->tl);
		}

		printf("goto %s\n", lab1->name);
		printf("%s:\n", lab3->name);
		return newType;

	case 'F':
		cond = newTypeCreate();
		lab1 = newLabPrint();
		lab2 = newLabPrint();
		lab3 = newLabPrint();

		printf("%s:\n", lab1->name);
		if (((struct astFor *)a)->exp1)
			cond = intermediateCode(((struct astFor *)a)->exp1);

		printf("if ");
		if (cond->reg)
			printf("%s", cond->str);
		else if (cond->str || cond->num)
			printf("%s", ((struct symref *)((struct astFor *)a))->s->name);

		printf(" < ");
		cond = intermediateCode(((struct astFor *)a)->exp2);

		if (cond->reg || cond->str)
			printf("%s", cond->str);
		else
			printf("%d", cond->num);

		printf(" goto %s\ngoto %s\n%s:\n", lab2->name, lab3->name, lab2->name);
		if (((struct astFor *)a)->tl) {
			intermediateCode(((struct astFor *)a)->tl);
		}

		newType = newTypePrint();
		printf("%s + 1\n", ((struct symref *)((struct astFor *)a))->s->name);
		printf("%s = %s\n", ((struct symref *)((struct astFor *)a))->s->name, newType->str);

		printf("goto %s\n", lab1->name);
		printf("%s:\n", lab3->name);
		return newType;

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
